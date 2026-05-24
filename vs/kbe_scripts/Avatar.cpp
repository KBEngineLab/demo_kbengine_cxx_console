#include "Avatar.h"
#include "../kbe_cxx_plugins/AvatarBase.h"
#include "../kbe_cxx_plugins/Entity.h"
#include "../kbe_cxx_plugins/EntityFactory.h"

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
	
}


void Avatar::reqRelive(uint8 reliveType)
{
	DEBUG_MSG("Avatar::reqRelive(): reqRelive");
}

void Avatar::onDestroy()
{
	AvatarBase::onDestroy();

	
}

void Avatar::onEnterWorld()
{
	AvatarBase::onEnterWorld();

	
}

// 玩家自己：onEnterWorld → addSpaceGeometryMapping → onEnterSpace
// 其他实体：addSpaceGeometryMapping → onEnterWorld
void Avatar::onLeaveWorld()
{
	AvatarBase::onLeaveWorld();


}

// 玩家自己：onEnterWorld → addSpaceGeometryMapping → onEnterSpace
// 其他实体：addSpaceGeometryMapping → onEnterWorld
void Avatar::onEnterSpace()
{
	AvatarBase::onEnterSpace();


	
}

void Avatar::onLeaveSpace()
{
	AvatarBase::onLeaveSpace();
}

void Avatar::onPositionChanged(const KBVector3f& oldValue)
{
	AvatarBase::onPositionChanged(oldValue);
	
}

void Avatar::onSmoothPositionChanged(const KBVector3f& oldValue)
{
	AvatarBase::onSmoothPositionChanged(oldValue);

}

void Avatar::onDirectionChanged(const KBVector3f& oldValue)
{
	AvatarBase::onDirectionChanged(oldValue);

	
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

}

void Avatar::onHPChanged(int32 oldValue)
{
	
}

void Avatar::onHP_MaxChanged(int32 oldValue)
{
	
}

void Avatar::onMPChanged(int32 oldValue)
{
	
}

void Avatar::onMP_MaxChanged(int32 oldValue)
{
	
}

void Avatar::onForbidsChanged(int32 oldValue)
{
	
}

void Avatar::onLevelChanged(uint16 oldValue)
{

}

void Avatar::onModelIDChanged(uint32 oldValue)
{
	
}

void Avatar::onModelScaleChanged(uint8 oldValue)
{
	
}

void Avatar::onMoveSpeedChanged(uint8 oldValue)
{
	
}

void Avatar::onNameChanged(const KBString& oldValue)
{
	
}

void Avatar::onOwn_valChanged(uint16 oldValue)
{

}

void Avatar::onSpaceUTypeChanged(uint32 oldValue)
{

}

void Avatar::onStateChanged(int8 oldValue)
{
	
}

void Avatar::onSubStateChanged(uint8 oldValue)
{
	
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
	const bool registeredAvatar = []() {
		EntityFactory::instance().registerType("Avatar", []() {
			return new KBEngine::Avatar();
		});
		return true;
	}();
}