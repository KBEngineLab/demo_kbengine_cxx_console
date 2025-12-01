// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "../kbengine_cxx_plugins/KBECommon.h"

#include "../kbengine_cxx_plugins/GateBase.h"

namespace KBEngine
{

class Gate : public GateBase
{
public:
	Gate();
	~Gate() override;

	void __init__() override;

};

}
