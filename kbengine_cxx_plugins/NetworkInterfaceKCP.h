// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ikcp.h"
#include "KBECommon.h"

#include "NetworkInterfaceBase.h"
#include "UdpClient.h"

/*
	网络模块
	处理连接、收发数据
*/
namespace KBEngine
{

class NetworkInterfaceKCP : public NetworkInterfaceBase
{
public:
	NetworkInterfaceKCP();
	virtual ~NetworkInterfaceKCP();

	bool connectTo(const KBString& addr, uint16 port, InterfaceConnect* callback, int userdata) override;
	void reset() override;
	void close() override;
	bool valid() override;
	bool sendTo(MemoryStream* pMemoryStream) override;

	virtual void process() override;

	ikcpcb*	pKCP() {
		return kcp_;
	}


protected:
	ikcpcb* kcp_ = nullptr;
	uint32 connID_ = 0;
	uint32_t nextKcpUpdate_ = 0;


	bool initKCP();
	bool finiKCP();

private:
	std::shared_ptr<hv::UdpClient> socket_;
};

}