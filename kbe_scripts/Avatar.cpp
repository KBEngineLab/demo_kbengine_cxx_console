#include "Avatar.h"
#include "../kbengine_cxx_plugins/Entity.h"
#include "LogicEvents.h"

#include "../kbengine_cxx_plugins/EntityFactory.h"

namespace KBEngine
{

Avatar::Avatar():
	AvatarBase()
{
}

Avatar::~Avatar()
{
}

void Avatar::__init__()
{
	// 注册事件
	if (isPlayer())
	{
		KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("relive", "relive", [this](std::shared_ptr<UKBEventData> pEventData)
		{
			const UKBEventData_reqRelive& data = static_cast<const UKBEventData_reqRelive&>(*pEventData);
			reqRelive(data.reliveType);
		});
	}
}


void Avatar::reqRelive(uint8 reliveType)
{
	DEBUG_MSG("Avatar::reqRelive(): reqRelive");
	pCellEntityCall->relive(reliveType);
}

void Avatar::onDestroy()
{
	AvatarBase::onDestroy();

	// 注销注册的所有事件
	KBENGINE_DEREGISTER_ALL_EVENT();
}

void Avatar::onEnterWorld()
{
	AvatarBase::onEnterWorld();
}

void Avatar::onLeaveWorld()
{
	AvatarBase::onLeaveWorld();
}

void Avatar::onEnterSpace()
{
	AvatarBase::onEnterSpace();
}

void Avatar::onLeaveSpace()
{
	AvatarBase::onLeaveSpace();
}

void Avatar::onAddSkill(int32 arg1)
{

}

void Avatar::onJump()
{

}

void Avatar::onRemoveSkill(int32 arg1)
{

}

void Avatar::recvDamage(int32 arg1, int32 arg2, int32 arg3, int32 arg4)
{

	auto pEventData = std::make_shared<UKBEventData_recvDamage>();
	pEventData->attackerID = arg1;
	pEventData->skillID = arg2;
	pEventData->damageType = arg3;
	pEventData->damage = arg4;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("recvDamage", pEventData);
}

void Avatar::onHPChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_HP>();
	pEventData->HP = HP;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_HP", pEventData);
}

void Avatar::onHP_MaxChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_HP_Max>();
	pEventData->HP_Max = HP_Max;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_HP_Max", pEventData);
}

void Avatar::onMPChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_MP>();
	pEventData->MP = MP;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_MP", pEventData);
}

void Avatar::onMP_MaxChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_MP_Max>();
	pEventData->MP_Max = MP_Max;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_MP_Max", pEventData);
}

void Avatar::onForbidsChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_forbids>();
	pEventData->forbids = forbids;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_forbids", pEventData);
}

void Avatar::onLevelChanged(uint16 oldValue)
{

}

void Avatar::onModelIDChanged(uint32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_modelID>();
	pEventData->modelID = modelID;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_modelID", pEventData);
}

void Avatar::onModelScaleChanged(uint8 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_modelScale>();
	pEventData->modelScale = modelScale;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_modelScale", pEventData);
}

void Avatar::onMoveSpeedChanged(uint8 oldValue)
{
	velocity(moveSpeed);

	auto pEventData = std::make_shared<UKBEventData_set_moveSpeed>();
	pEventData->moveSpeed = moveSpeed;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_moveSpeed", pEventData);
}

void Avatar::onNameChanged(const KBString& oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_name>();
	pEventData->name = name;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_name", pEventData);
}

void Avatar::onOwn_valChanged(uint16 oldValue)
{

}

void Avatar::onSpaceUTypeChanged(uint32 oldValue)
{

}

void Avatar::onStateChanged(int8 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_state>();
	pEventData->state = state;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_state", pEventData);
}

void Avatar::onSubStateChanged(uint8 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_subState>();
	pEventData->subState = subState;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_subState", pEventData);
}

void Avatar::onUidChanged(uint32 oldValue)
{

}

void Avatar::onUtypeChanged(uint32 oldValue)
{

}

}


// 静态注册
namespace {
	const bool registered = []() {
		EntityFactory::instance().registerType("Avatar", []() {
			return new KBEngine::Avatar();
		});
		return true;
	}();
}