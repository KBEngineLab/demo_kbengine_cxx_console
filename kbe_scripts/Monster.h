// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "../kbengine_cxx_plugins/KBECommon.h"

#include "../kbengine_cxx_plugins/MonsterBase.h"

namespace KBEngine
{

class Monster : public MonsterBase
{
public:
	Monster();
	virtual ~Monster();

	void __init__() override;

	void recvDamage(int32 arg1, int32 arg2, int32 arg3, int32 arg4) override;

};

}
