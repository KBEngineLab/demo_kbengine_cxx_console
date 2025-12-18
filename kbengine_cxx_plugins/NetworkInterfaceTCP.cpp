
#include "NetworkInterfaceTCP.h"

#include "GameThreadDispatcher.h"
#include "MemoryStream.h"
#include "KBEvent.h"
#include "KBDebug.h"
#include "Interfaces.h"
#include "MessageReader.h"

namespace KBEngine
{

NetworkInterfaceTCP::NetworkInterfaceTCP(): NetworkInterfaceBase()
{
	// DEBUG_MSG("NetworkInterfaceTCP::NetworkInterfaceTCP():");
}

NetworkInterfaceTCP::~NetworkInterfaceTCP()
{
	// DEBUG_MSG("NetworkInterfaceTCP::~NetworkInterfaceTCP():");
	NetworkInterfaceTCP::reset();
}

bool NetworkInterfaceTCP::connectTo(const KBString &addr, uint16 port, InterfaceConnect *callback, int userdata) {
	INFO_MSG("NetworkInterfaceTCP::connectTo(): will connect to %s:%d ...", addr.c_str(), port);
	reset();

	connectCB_ = callback;
	connectIP_ = addr;
	connectPort_ = port;
	connectUserdata_ = userdata;
	startTime_ = getTimeSeconds();


	// Loop = std::make_shared<hv::EventLoopThread>();

	// Loop->start();
	// 如果 socket_ 没创建，就创建 TcpClient
	if (!socket_) {
		socket_ = std::make_shared<hv::TcpClient>();
	}




	// printf(TCHARToANSI(*addr).c_str());
	int ret = socket_->createsocket(port, TCHARToANSI(*addr).c_str());
	if (ret < 0) {
		ERROR_MSG("NetworkInterfaceBase::connectTo(): createsocket failed %d", ret);
		return false;
	}


	socket_->setConnectTimeout(30000);

	// 30秒超时
	// startTime_ = hv::gettimeofday();

	// 连接成功回调
	socket_->onConnection = [this](const hv::SocketChannelPtr& ch) {
		// printf("connectCB_: %d  %d %d\n", ch->isConnected(),ch->isClosed(),ch->isOpened());

		if (connectCB_) {
			if (ch->isConnected()) {
				INFO_MSG("NetworkInterfaceTCP::connectTo(): connect to  %s success!", ch->peeraddr().c_str());
				auto connectCB = connectCB_;
				GameThreadDispatcher::Instance().Post(
				   [this, connectCB]() mutable
				   {
				   		connectCB->onConnectCallback(connectIP_, connectPort_, true, connectUserdata_);
						// connectCB_ = nullptr;
						connectCB = nullptr;

						auto pEventData = std::make_shared<UKBEventData_onConnectionState>();
						pEventData->success = true;
						pEventData->address = KBString::Printf(KBTEXT("%s:%d"), *connectIP_, connectPort_);
						KBENGINE_EVENT_FIRE(KBEventTypes::onConnectionState, pEventData);

				   }
			    );
				connectCB_ = nullptr;

			}else {
				// 连接超时或者无法连接
				ERROR_MSG("NetworkInterfaceTCP::connectTo(): connect to %s timeout!", ch->peeraddr().c_str());
				auto connectCB = connectCB_;
				connectCB_ = nullptr;
				GameThreadDispatcher::Instance().Post(
				   [this,connectCB]() mutable
				   {
					   connectCB->onConnectCallback(connectIP_, connectPort_, false, connectUserdata_);
					   connectCB = nullptr;

					   auto pEventData = std::make_shared<UKBEventData_onConnectionState>();
					   pEventData->success = false;
					   pEventData->address = KBString::Printf(KBTEXT("%s:%d"), *connectIP_, connectPort_);
					   KBENGINE_EVENT_FIRE(KBEventTypes::onConnectionState, pEventData);
				   }
			   );
			}

		}


	};


	// 数据回调
	socket_->onMessage = [this](const hv::SocketChannelPtr& ch, hv::Buffer* buf) {

		const auto* data  = reinterpret_cast<const uint8_t*>(buf->data());
		size_t len = buf->size();
		pBuffer_->clear(true);
		pBuffer_->append(data, len);

		if (pFilter_) {
			pFilter_->recv(pMessageReader_,pBuffer_);
		}else {
			pMessageReader_->process(pBuffer_->data(), 0, len);
		}


		// const auto* begin  = reinterpret_cast<const uint8_t*>(buf->data());
		// size_t len = buf->size();
		// std::vector<uint8_t> data(begin , begin  + len);
		// // 这里要改，应该操作完io后再投递进去
		// GameThreadDispatcher::Instance().Post(
		// 	[this, data = std::move(data), len]() mutable
		// 	{
		//
		// 		MemoryStream pBuffer;
		// 		pBuffer.clear(true);
		// 		pBuffer.append(data.data(), data.size());
		//
		//
		// 		if (pFilter_) {
		// 			pFilter_->recv(pMessageReader_,&pBuffer);
		// 		}else {
		// 			pMessageReader_->process(pBuffer.data(), 0, len);
		// 		}
		// 	}
		// );



	};
	// 新建一个事件循环对象
	socket_->start();  // 启动 EventLoop



	// socket_->startConnect();

	return true;
}

void NetworkInterfaceTCP::reset() {

	if (socket_)
	{

		socket_->stop();
		socket_->closesocket();
		// socket_->onMessage = nullptr;
		// socket_->onConnection = nullptr;
		INFO_MSG("NetworkInterfaceTCP::reset(): network reset!");
		socket_ = nullptr;
	}

	connectCB_ = nullptr;
	connectIP_ = KBTEXT("");
	connectPort_ = 0;
	connectUserdata_ = 0;
	startTime_ = 0.0;
}

void NetworkInterfaceTCP::close() {

	if (socket_)
	{

		socket_->stop();
		socket_->closesocket();
		// socket_->onMessage = nullptr;
		// socket_->onConnection = nullptr;
		INFO_MSG("NetworkInterfaceTCP::close(): network closed!");
		KBENGINE_EVENT_FIRE(KBEventTypes::onDisconnected, std::make_shared<UKBEventData_onDisconnected>());
		socket_ = nullptr;
	}
	
	KBE_SAFE_RELEASE(pMessageReader_);
	KBE_SAFE_RELEASE(pBuffer_);
	KBE_SAFE_RELEASE(pFilter_);
	connectCB_ = nullptr;
	connectIP_ = KBTEXT("");
	connectPort_ = 0;
	connectUserdata_ = 0;
	startTime_ = 0.0;




}

bool NetworkInterfaceTCP::valid() {
	return socket_ != nullptr;
}


bool NetworkInterfaceTCP::sendTo(MemoryStream *pMemoryStream) {
	if (!socket_) {
		ERROR_MSG("NetworkInterfaceKCP::sendTo(): socket is null!");
		return false;
	}

	if (!pMemoryStream || pMemoryStream->length() == 0)
		return true;

	int ret = socket_->send(pMemoryStream->data(), pMemoryStream->length());
	if (ret < 0) {
		ERROR_MSG("NetworkInterfaceKCP::sendTo(): sendto failed ret=%d", ret);
		return false;
	}

	return true;
	// return socket_->send(pMemoryStream->data(),pMemoryStream->length());
}

void NetworkInterfaceTCP::process()
{
	// if (Loop) {
	// 	hloop_process_events(Loop->loop(),0); // 手动驱动一次事件循环
	// }
}

}
