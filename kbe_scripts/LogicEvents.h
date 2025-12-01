// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "../kbengine_cxx_plugins/KBECommon.h"
#include "../kbengine_cxx_plugins/KBEvent.h"

/**
	所有由逻辑层产生的事件都在这里定义
*/

struct FAVATAR_INFOS
{

	uint64 dbid;

	// 由于蓝图不可读取uint64类型， 因此提供一个字符串类型用于比较和读取
	KBString sdbid;

	void set(DBID _dbid, const KBString& _name, uint8 _roleType, int _level, bool _lastSelCharacter)
	{
		name = _name;
		dbid = _dbid;
		sdbid = KBString::Printf(KBTEXT("%lld"), dbid);
		roleType = _roleType;
		level = _level;
		lastSelCharacter = _lastSelCharacter;
	}

	KBString name;

	uint8 roleType;

	int level;

	// 服务端记录的最后一次所选择的角色
	bool lastSelCharacter;
};

class UKBEventData_reqCreateAvatar : public UKBEventData
{
public:
	uint8 roleType;

	KBString name;
};

class UKBEventData_onReqAvatarList : public UKBEventData
{
public:
	KBArray<FAVATAR_INFOS> avatars;
};

class UKBEventData_reqRemoveAvatar : public UKBEventData
{
public:
	FAVATAR_INFOS avatarInfos;
};

class UKBEventData_onRemoveAvatar : public UKBEventData
{
public:
	FAVATAR_INFOS avatarInfos;
};

class UKBEventData_onCreateAvatarResult : public UKBEventData
{
public:
	int errorCode;

	KBString errorStr;

	FAVATAR_INFOS avatarInfos;
};

class UKBEventData_selectAvatarGame : public UKBEventData
{
public:
	FAVATAR_INFOS avatarInfos;
};


class UKBEventData_set_moveSpeed : public UKBEventData
{
public:
	int entityID;

	float moveSpeed;
};

class UKBEventData_set_entityNO : public UKBEventData
{

public:
	int entityID;

	int32 entityNO;
};

class UKBEventData_set_name : public UKBEventData
{

public:
	int entityID;

	KBString name;
};

class UKBEventData_set_modelID : public UKBEventData
{
public:
	uint32 modelID;

	int entityID;
};

class UKBEventData_set_modelScale : public UKBEventData
{
public:
	uint8 modelScale;

	int entityID;
};

class UKBEventData_set_HP : public UKBEventData
{
public:
	int32 HP;

	int entityID;
};

class UKBEventData_set_HP_Max : public UKBEventData
{

public:
	int32 HP_Max;

	int entityID;
};

class UKBEventData_set_MP : public UKBEventData
{
public:
	int32 MP;

	int entityID;
};

class UKBEventData_MP_Max : public UKBEventData
{
public:
	int32 MP_Max;

	int entityID;
};




class UKBEventData_set_flags : public UKBEventData
{
public:
	int32 flags;

	int entityID;
};


class UKBEventData_set_state : public UKBEventData
{
	

public:
	int32 state;

	int entityID;
};


class UKBEventData_set_subState : public UKBEventData
{
	

public:
	int32 subState;

	int entityID;
};


class UKBEventData_set_forbids : public UKBEventData
{
	

public:
	int32 forbids;

	int entityID;
};


class UKBEventData_reqRelive : public UKBEventData
{
	

public:
	uint8 reliveType;
};


class UKBEventData_recvDamage : public UKBEventData
{
	

public:
	int32 attackerID;

	int32 skillID;

	int32 damageType;

	int32 damage;

	int entityID;
};