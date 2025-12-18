
#include "NetworkInterfaceKCP.h"

#include "GameThreadDispatcher.h"
#include "MemoryStream.h"
#include "KBEvent.h"
#include "KBDebug.h"
#include "Interfaces.h"
#include "KBEngine.h"
#include "KBEngineArgs.h"
#include "MessageReader.h"
#include "htime.h"

namespace KBEngine
{

NetworkInterfaceKCP::NetworkInterfaceKCP():
	NetworkInterfaceBase()
{

}

NetworkInterfaceKCP::~NetworkInterfaceKCP()
{
	// NetworkInterfaceKCP::close();
	NetworkInterfaceKCP::reset();
}

bool NetworkInterfaceKCP::connectTo(const KBString &addr, uint16 port, InterfaceConnect *callback, int userdata) {
	INFO_MSG("NetworkInterfaceKCP::connectTo(): will connect to %s:%d ...", *addr, port);
	reset();

	// Loop = std::make_shared<hv::EventLoop>();

	connectCB_ = callback;
	connectIP_ = addr;
	connectPort_ = port;
	connectUserdata_ = userdata;
	startTime_ = getTimeSeconds();


	// 如果 socket_ 没创建，就创建 TcpClient
	if (!socket_) {
		socket_ = std::make_shared<hv::UdpClient>();
		// 不用内置的kcp，协议不匹配
		// kcp_setting_t stSetting;
		// kcp_setting_init_with_normal_mode(&stSetting);
		// socket_->setKcp(&stSetting);
	}
	// 数据回调
	socket_->onMessage = [this](const hv::SocketChannelPtr& ch, hv::Buffer* buf) {

		if (connectCB_) {
			MemoryStream ms;
			const auto* data = reinterpret_cast<const uint8_t*>(buf->data());
			ms.append(data, buf->size());
			ms.rpos(0);

			KBString helloAck, versionString;
			uint32 connID;

			ms >> helloAck >> versionString >> connID;


			bool success = true;

			if (helloAck != UDP_HELLO_ACK) {
				ERROR_MSG("NetworkInterfaceKCP::connectTo(): failed to connect to '%s:%d'! receive hello-ack(%s!=%s) mismatch!",
						  *connectIP_, connectPort_, *helloAck, *UDP_HELLO_ACK);
				success = false;
			}
			else if (KBEngineApp::getSingleton().serverVersion() != versionString) {
				ERROR_MSG("NetworkInterfaceKCP::connectTo(): failed to connect to '%s:%d'! version(%s!=%s) mismatch!",
						  *connectIP_, connectPort_, *versionString, *KBEngineApp::getSingleton().serverVersion());
				success = false;
			}
			else if (connID == 0) {
				ERROR_MSG("NetworkInterfaceKCP::connectTo(): failed to connect to '%s:%d'! conv is 0!",
						  *connectIP_, connectPort_);
				success = false;
			}
			else {
				INFO_MSG("NetworkInterfaceKCP::connectTo(): connect to %s:%d success!", *connectIP_, connectPort_);
				connID_ = connID;
				initKCP();
			}


			auto connectCB = connectCB_;
			GameThreadDispatcher::Instance().Post(
			   [this,connectCB,success]() mutable
			   {
					connectCB->onConnectCallback(connectIP_, connectPort_, success, connectUserdata_);
					connectCB = nullptr;



					auto pEventData = std::make_shared<UKBEventData_onConnectionState>();
					pEventData->success = success;
					pEventData->address = KBString::Printf(KBTEXT("%s:%d"), *connectIP_, connectPort_);
					KBENGINE_EVENT_FIRE(KBEventTypes::onConnectionState, pEventData);
			   }
		   );

			connectCB_ = nullptr;


		}else {
			const char* data = (const char*)buf->data();
			size_t len = buf->size();

			if (kcp_)
				ikcp_input(kcp_, data, (long)len);

			// 尝试从 KCP 收数据（可能一次收到多个）
			char recvBuf[65536];
			int recvLen = ikcp_recv(kcp_, recvBuf, sizeof(recvBuf));
			while (recvLen > 0)
			{
				pBuffer_->clear(true);
				pBuffer_->append((uint8*)recvBuf, recvLen);

				if (pFilter_)
					pFilter_->recv(pMessageReader_, pBuffer_);
				else
					pMessageReader_->process(pBuffer_->data(), 0, recvLen);

				recvLen = ikcp_recv(kcp_, recvBuf, sizeof(recvBuf));
			}
		}

	};

	int ret = socket_->createsocket(port, TCHARToANSI(*addr).c_str());
	if (ret < 0) {
		ERROR_MSG("NetworkInterfaceBase::connectTo(): createsocket failed %d", ret);
		return false;
	}

	socket_->start();  // 启动 EventLoop

	// socket_->startRecv();
	// socket_->loop()->setTimeout()
	socket_->loop()->setInterval(10, [this](hv::TimerID timerID) {
		if (!kcp_) return;
		uint32_t now = gettimeofday_ms();
		if (now >= nextKcpUpdate_) {
			ikcp_update(kcp_, now);
			nextKcpUpdate_ = ikcp_check(kcp_, now);
		}
	});
	socket_->sendto(UDP_HELLO);


	return true;
}

void NetworkInterfaceKCP::reset()
{
	// NetworkInterfaceBase::reset();
	finiKCP();
	if (socket_)
	{
		socket_->stop();
		socket_->closesocket();
		INFO_MSG("NetworkInterfaceKCP::reset(): network reset!");
		socket_ = nullptr;
	}

	socket_ = nullptr;

	// KBE_SAFE_RELEASE(pMessageReader_);
	// KBE_SAFE_RELEASE(pFilter_);

	connectCB_ = nullptr;
	connectIP_ = KBTEXT("");
	connectPort_ = 0;
	connectUserdata_ = 0;
	startTime_ = 0.0;
}

void NetworkInterfaceKCP::close()
{
	finiKCP();
	if (socket_)
	{
		socket_->stop();
		socket_->closesocket();
		INFO_MSG("NetworkInterfaceKCP::close(): network closed!");
		KBENGINE_EVENT_FIRE(KBEventTypes::onDisconnected, std::make_shared<UKBEventData_onDisconnected>());
		socket_ = nullptr;
	}

	socket_ = nullptr;

	KBE_SAFE_RELEASE(pMessageReader_);
	KBE_SAFE_RELEASE(pBuffer_);
	KBE_SAFE_RELEASE(pFilter_);
	
	connectCB_ = nullptr;
	connectIP_ = KBTEXT("");
	connectPort_ = 0;
	connectUserdata_ = 0;
	startTime_ = 0.0;

}

bool NetworkInterfaceKCP::valid()
{
	return socket_ && (pKCP() || connectCB_);
}

bool NetworkInterfaceKCP::sendTo(MemoryStream *pMemoryStream) {
	if (!socket_) {
		ERROR_MSG("NetworkInterfaceKCP::sendTo(): socket is null!");
		return false;
	}

	if (!kcp_ || !pMemoryStream || pMemoryStream->length() == 0)
		return false;

	ikcp_send(kcp_, (const char*)pMemoryStream->data(), pMemoryStream->length());
	return true;

}

void NetworkInterfaceKCP::process()
{
	if (!kcp_) return;
	uint32_t now = gettimeofday_ms();
	if (now >= nextKcpUpdate_) {
		ikcp_update(kcp_, now);
		nextKcpUpdate_ = ikcp_check(kcp_, now);
	}
}

bool NetworkInterfaceKCP::initKCP() {

	if (kcp_)
	{
		ikcp_release(kcp_);
		kcp_ = nullptr;
	}


	kcp_ = ikcp_create((IUINT32)connID_, (void*)this);
	// kcp_->output = &NetworkInterfaceKCP::kcp_output;
	kcp_->output = [](const char* buf, int len, ikcpcb* kcp, void* user) -> int
	{
		auto* self = reinterpret_cast<NetworkInterfaceKCP*>(user);
		if (!self->socket_) return 0;
		return self->socket_->sendto(buf, len);
	};

	ikcp_setmtu(kcp_, 1400);


	ikcp_wndsize(kcp_, KBEngineApp::getSingleton().getInitArgs()->getUDPSendBufferSize(), KBEngineApp::getSingleton().getInitArgs()->getUDPRecvBufferSize());
	ikcp_nodelay(kcp_, 1, 10, 2, 1);
	kcp_->rx_minrto = 10;
	// nextKcpUpdate_ = 0;
	nextKcpUpdate_ = gettimeofday_ms();
	return true;
}

bool NetworkInterfaceKCP::finiKCP() {
	if (!kcp_)
		return true;

	ikcp_release(kcp_);
	kcp_ = nullptr;

	return true;
}
}
