#include "Monster.h"
#include "../kbe_cxx_plugins/Entity.h"
#include "LogicEvents.h"
#include "../kbe_cxx_plugins/EntityFactory.h"


namespace KBEngine
{

Monster::Monster():
	MonsterBase()
{
}

Monster::~Monster()
{
}

void Monster::__init__()
{
	
}

void Monster::recvDamage(int32 arg1, int32 arg2, int32 arg3, int32 arg4) {
}

void Monster::onDestroy()
{
	MonsterBase::onDestroy();

}

void Monster::onLeaveWorld()
{
	MonsterBase::onLeaveWorld();

	
}

void Monster::onLeaveSpace()
{
	MonsterBase::onLeaveSpace();
	
}

void Monster::onEnterWorld()
{
	MonsterBase::onEnterWorld();

}

void Monster::onEnterSpace()
{
	MonsterBase::onEnterSpace();
}

void Monster::onPositionChanged(const KBVector3f& oldValue)
{
	MonsterBase::onPositionChanged(oldValue);


}

void Monster::onSmoothPositionChanged(const KBVector3f& oldValue)
{
	MonsterBase::onSmoothPositionChanged(oldValue);
	
}

void Monster::onDirectionChanged(const KBVector3f& oldValue)
{
	MonsterBase::onDirectionChanged(oldValue);
	
}

void Monster::onMoveSpeedChanged(uint8 oldValue)
{
	MonsterBase::onMoveSpeedChanged(oldValue);
	
}
}


// 静态注册
namespace {
	const bool registeredMonster = []() {
		EntityFactory::instance().registerType("Monster", []() {
			return new KBEngine::Monster();
		});
		return true;
	}();
}