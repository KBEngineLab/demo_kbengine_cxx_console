// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "../kbe_cxx_plugins/KBECommon.h"

#include "../kbe_cxx_plugins/MonsterBase.h"

namespace KBEngine
{

class Monster : public MonsterBase
{
public:
	Monster();
	virtual ~Monster();

	void __init__() override;

	void recvDamage(int32 arg1, int32 arg2, int32 arg3, int32 arg4) override;

	virtual void onDestroy() override;
	virtual void onLeaveWorld() override;
	virtual void onLeaveSpace() override;

	virtual void onEnterWorld() override;
	virtual void onEnterSpace() override;



	virtual void onPositionChanged(const KBVector3f& oldValue) override;
	virtual void onSmoothPositionChanged(const KBVector3f& oldValue) override;
	virtual void onDirectionChanged(const KBVector3f& oldValue) override;
	virtual void onMoveSpeedChanged(uint8 oldValue) override;
	

};

}
