#include "Account.h"

#include "../kbe_cxx_plugins/AccountBase.h"
#include "../kbe_cxx_plugins/Entity.h"
#include "../kbe_cxx_plugins/KBEngine.h"
#include "../kbe_cxx_plugins/KBDebug.h"
#include "LogicEvents.h"
#include "../kbe_cxx_plugins/EntityFactory.h"


namespace KBEngine
{
Account* Account::Instance = nullptr;
Account::Account():
	AccountBase()
{

	Instance = this;
}

Account::~Account()
{

	Instance = nullptr;
}

void Account::__init__()
{

	// 触发登陆成功事件
	auto pEventData = std::make_shared<UKBEventData_onLoginSuccessfully>();
	pEventData->entity_uuid = KBEngineApp::getSingleton().entity_uuid();
	pEventData->entity_id = id();
	KBENGINE_EVENT_FIRE_OUT("onLoginSuccessfully", pEventData);

	// 向服务端请求获得角色列表
	pBaseEntityCall->reqAvatarList();

}

void Account::onDestroy()
{
	// 注销注册的所有事件
	KBENGINE_DEREGISTER_ALL_EVENT();

	AccountBase::onDestroy();
}

void Account::onLastSelCharacterChanged(uint64 oldValue)
{

}

void Account::reqCreateAvatar(uint8 roleType, const KBString& name)
{
	DEBUG_MSG("Account::reqCreateAvatar(): roleType=%d", roleType);
	pBaseEntityCall->reqCreateAvatar(roleType, name);
}

void Account::reqRemoveAvatar(uint64 dbid)
{
	DEBUG_MSG("Account::reqRemoveAvatar(): dbid=%lld", dbid);
	pBaseEntityCall->reqRemoveAvatarDBID(dbid);
}

void Account::selectAvatarGame(uint64 dbid)
{
	DEBUG_MSG("Account::selectAvatarGame(): dbid=%lld", dbid);
	pBaseEntityCall->selectAvatarGame(dbid);
}

void Account::onReqAvatarList(const AVATAR_INFOS_LIST& datas)
{
	DEBUG_MSG("Account::onReqAvatarList(): datas.size()=%d", datas.values.Num());
	// auto pEventData = std::make_shared<UKBEventData_onReqAvatarList>();
	// for (auto& characterInfoItem : datas.values)
	// {
	// 	FAVATAR_INFOS event_avatar;
	//
	// 	const AVATAR_INFOS& characterInfo_fixed_dict = characterInfoItem;
	//
	// 	DEBUG_MSG("Account::onReqAvatarList(): name=%s, dbid=%lld, level=%d, roleType=%d", characterInfo_fixed_dict.name.c_str(), characterInfo_fixed_dict.dbid, characterInfo_fixed_dict.level, characterInfo_fixed_dict.roleType);
	//
	// 	AVATAR_INFOS infos;
	//
	// 	infos.name = characterInfo_fixed_dict.name;
	// 	infos.dbid = characterInfo_fixed_dict.dbid;
	// 	infos.level = characterInfo_fixed_dict.level;
	// 	infos.roleType = characterInfo_fixed_dict.roleType;
	//
	// 	const AVATAR_DATA& data_fixed_dict = characterInfo_fixed_dict.data;
	//
	// 	infos.data.param1 = data_fixed_dict.param1;
	// 	infos.data.param2 = data_fixed_dict.param2;
	//
	// 	characters.values.Add(infos);
	//
	// 	// fill eventData
	// 	event_avatar.set(infos.dbid, infos.name, infos.roleType, infos.level, (lastSelCharacter == infos.dbid));
	// 	pEventData->avatars.Add(event_avatar);
	// }
	//
	// KBENGINE_EVENT_FIRE("onReqAvatarList", pEventData);
}

void Account::onCreateAvatarResult(uint8 retcode, const AVATAR_INFOS& info)
{
	DEBUG_MSG("Account::onCreateAvatarResult(): retcode=%d", retcode);
	
}

void Account::onRemoveAvatar(uint64 dbid)
{
	DEBUG_MSG("Account::onRemoveAvatar(): dbid=%lld", dbid);
	pBaseEntityCall->reqAvatarList();
}

}


// 静态注册
namespace {
	const bool registered = []() {
		EntityFactory::instance().registerType("Account", []() {
			return new KBEngine::Account();
		});
		return true;
	}();
}