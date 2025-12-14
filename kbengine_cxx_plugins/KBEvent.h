// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <functional>
#include "KBECommon.h"
#include "KBEventTypes.h"


/*
事件模块
事件的数据基础类， 不同事件需要实现不同的数据类
*/
class UKBEventData
{
public:
	// 事件名称，可用于对事件类型进行判断，该名称由事件触发时事件系统进行填充
	KBString eventName;
};

/*
	事件模块
	KBEngine插件层与Unity3D表现层通过事件来交互
*/
class KBEvent
{
public:
	KBEvent();
	virtual ~KBEvent();
	
public:
		static bool registerEvent(const KBString& eventName, const KBString& funcName, std::function<void(std::shared_ptr<UKBEventData>)> func, void* objPtr);
		static bool deregister(void* objPtr, const KBString& eventName, const KBString& funcName);
		static bool deregister(void* objPtr);

		// static void fire(const KBString& eventName, UKBEventData* pEventData);
		static void fire(const KBString& eventName, std::shared_ptr<UKBEventData> pEventData);

		static void clear();
		static void clearFiredEvents();

		static void processInEvents() {}
		static void processOutEvents() {}

		static void pause();
		static void resume();

		static void removeFiredEvent(void* objPtr, const KBString& eventName = KBTEXT(""), const KBString& funcName = KBTEXT(""));

protected:
	struct EventObj
	{
		std::function<void(std::shared_ptr<UKBEventData>)> method;
		KBString funcName;
		void* objPtr;
	};

	struct FiredEvent
	{
		EventObj evt;
		KBString eventName;
		std::shared_ptr<UKBEventData> args;
	};

	static KBMap<KBString, KBArray<EventObj>> events_;
	static KBArray<FiredEvent*> firedEvents_;
	static bool isPause_;
};


// 注册事件
#define KBENGINE_REGISTER_EVENT(EVENT_NAME, EVENT_FUNC) \
	KBEvent::registerEvent(EVENT_NAME, #EVENT_FUNC, [this](std::shared_ptr<UKBEventData> pEventData) {	EVENT_FUNC(pEventData);	}, (void*)this);

// 注册事件，可重写事件函数
#define KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC(EVENT_NAME, FUNC_NAME, EVENT_FUNC) \
	KBEvent::registerEvent(EVENT_NAME, FUNC_NAME, EVENT_FUNC, (void*)this);

// 注销这个对象某个事件
#define KBENGINE_DEREGISTER_EVENT_BY_FUNCNAME(EVENT_NAME, FUNC_NAME) KBEvent::deregister((void*)this, EVENT_NAME, FUNC_NAME);
#define KBENGINE_DEREGISTER_EVENT(EVENT_NAME) KBEvent::deregister((void*)this, EVENT_NAME, KBTEXT(""));

// 注销这个对象所有的事件
#define KBENGINE_DEREGISTER_ALL_EVENT()	KBEvent::deregister((void*)this);

// fire event
#define KBENGINE_EVENT_FIRE(EVENT_NAME, EVENT_DATA) KBEvent::fire(EVENT_NAME, EVENT_DATA);

// 暂停事件
#define KBENGINE_EVENT_PAUSE() KBEvent::pause();

// 恢复事件
#define KBENGINE_EVENT_RESUME() KBEvent::resume();

// 清除所有的事件
#define KBENGINE_EVENT_CLEAR() KBEvent::clear();

class UKBEventData_Baseapp_importClientMessages : public UKBEventData
{
public:
};

class UKBEventData_onKicked : public UKBEventData
{

public:
	int32 failedcode;

	KBString errorStr;
};

class UKBEventData_createAccount : public UKBEventData
{
public:
	KBString username;

	KBString password;

	KBArray<uint8> datas;
};

class UKBEventData_login : public UKBEventData
{

public:
	KBString username;

	KBString password;

	KBArray<uint8> datas;
};

class UKBEventData_logout : public UKBEventData
{
public:
};

class UKBEventData_onLoginFailed : public UKBEventData
{
public:
	int32 failedcode;

	KBString errorStr;

	KBArray<uint8> serverdatas;
};

class UKBEventData_onLoginBaseapp : public UKBEventData
{
public:
};

class UKBEventData_onLoginSuccessfully : public UKBEventData
{
public:
	uint64  entity_uuid;

	int32 entity_id;
};

class UKBEventData_onReloginBaseapp : public UKBEventData
{

public:
};

class UKBEventData_onLoginBaseappFailed : public UKBEventData
{

public:
	int32 failedcode;

	KBString errorStr;
};

class UKBEventData_onReloginBaseappFailed : public UKBEventData
{
public:

	int32 failedcode;

	KBString errorStr;
};

class UKBEventData_onReloginBaseappSuccessfully : public UKBEventData
{

public:
};

class UKBEventData_onVersionNotMatch : public UKBEventData
{

public:
	KBString clientVersion;
	
	KBString serverVersion;
};

class UKBEventData_onScriptVersionNotMatch : public UKBEventData
{

public:
	KBString clientScriptVersion;
	
	KBString serverScriptVersion;
};

class UKBEventData_Loginapp_importClientMessages : public UKBEventData
{

public:
};

class UKBEventData_Baseapp_importClientEntityDef : public UKBEventData
{

public:
};

class UKBEventData_onControlled : public UKBEventData
{

public:
	int entityID;

	bool isControlled;
};

class UKBEventData_onLoseControlledEntity : public UKBEventData
{
public:
	int entityID;
};

class UKBEventData_updatePosition : public UKBEventData
{
public:
	KBVector3f position;

	KBRotator direction;

	int entityID;

	float moveSpeed;

	bool isOnGround;
};

class UKBEventData_set_position : public UKBEventData
{
public:
	KBVector3f position;

	int entityID;

	float moveSpeed;

	bool isOnGround;
};

class UKBEventData_set_direction : public UKBEventData
{
public:
	// roll, pitch, yaw
	KBRotator direction;

	int entityID;
};

class UKBEventData_onCreateAccountResult : public UKBEventData
{

public:
	int errorCode;

	KBString errorStr;

	KBArray<uint8> datas;
};

class UKBEventData_addSpaceGeometryMapping : public UKBEventData
{
public:
	KBString spaceResPath;
};

class UKBEventData_onSetSpaceData : public UKBEventData
{
public:
	int spaceID;

	KBString key;

	KBString value;
};
class UKBEventData_onDelSpaceData : public UKBEventData
{

public:
	int spaceID;

	KBString key;
};

class UKBEventData_onDisconnected : public UKBEventData
{

public:
};

class UKBEventData_onConnectionState : public UKBEventData
{
public:
	bool success;

	KBString address;
};

class UKBEventData_onEnterWorld : public UKBEventData
{
public:
	KBString entityClassName;

	int spaceID;

	int entityID;

	KBString res;

	KBVector3f position;

	// roll, pitch, yaw
	KBVector3f direction;

	float moveSpeed;

	bool isOnGround;

	bool isPlayer;
};

class UKBEventData_onLeaveWorld : public UKBEventData
{
public:
	int spaceID;

	int entityID;

	bool isPlayer;
};

class UKBEventData_onEnterSpace : public UKBEventData
{

public:
	KBString entityClassName;

	int spaceID;

	int entityID;

	KBString res;

	KBVector3f position;

	// roll, pitch, yaw
	KBVector3f direction;

	float moveSpeed;

	bool isOnGround;

	bool isPlayer;
};

class UKBEventData_onLeaveSpace : public UKBEventData
{
public:
	int spaceID;

	int entityID;

	bool isPlayer;
};

class UKBEventData_resetPassword : public UKBEventData
{

public:
	KBString username;
};

class UKBEventData_onResetPassword : public UKBEventData
{
public:
	int32 failedcode;

	KBString errorStr;
};

class UKBEventData_bindAccountEmail : public UKBEventData
{

public:
	KBString email;
};

class UKBEventData_onBindAccountEmail : public UKBEventData
{
public:
	int32 failedcode;

	KBString errorStr;
};

class UKBEventData_newPassword : public UKBEventData
{

public:
	KBString old_password;

	KBString new_password;
};

class UKBEventData_onNewPassword : public UKBEventData
{

public:
	int32 failedcode;

	KBString errorStr;
};

class UKBEventData_onStreamDataStarted : public UKBEventData
{

public:
	int resID;

	int dataSize;

	KBString dataDescr;
};

class UKBEventData_onStreamDataRecv : public UKBEventData
{

public:
	int resID;

	KBArray<uint8> data;
};

class UKBEventData_onStreamDataCompleted : public UKBEventData
{

public:
	int resID;
};

class UKBEventData_onImportClientSDK : public UKBEventData
{

public:
	int remainingFiles;

	int fileSize;

	KBString fileName;

	KBArray<uint8> fileDatas;
};

class UKBEventData_onImportClientSDKSuccessfully : public UKBEventData
{
public:

};

class UKBEventData_onDownloadSDK : public UKBEventData
{
public:
	bool isDownload;
};

