// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

#include "NetworkInterfaceBase.h"
#include "TcpClient.h"

/*
	网络模块
	处理连接、收发数据
*/
namespace KBEngine
{

class NetworkInterfaceTCP : public NetworkInterfaceBase
{
public:
	NetworkInterfaceTCP();
	~NetworkInterfaceTCP() override;


	bool connectTo(const KBString& addr, uint16 port, InterfaceConnect* callback, int userdata) override;
	void reset() override;
	void close() override;
	bool valid() override;
	bool sendTo(MemoryStream* pMemoryStream) override;

	virtual void process() override;

private:
	std::shared_ptr<hv::TcpClient> socket_;
};

}
