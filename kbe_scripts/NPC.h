// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "../kbengine_cxx_plugins/KBECommon.h"

#include "../kbengine_cxx_plugins/NPCBase.h"

namespace KBEngine
{

class NPC : public NPCBase
{
public:
	NPC();
	virtual ~NPC();

	void __init__() override;

};

}
