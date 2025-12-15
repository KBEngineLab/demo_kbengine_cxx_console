#include "Monster.h"
#include "../kbengine_cxx_plugins/Entity.h"
#include "LogicEvents.h"

#include "../kbengine_cxx_plugins/EntityFactory.h"


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
}


// 静态注册
namespace {
	const bool registered = []() {
		EntityFactory::instance().registerType("Monster", []() {
			return new  KBEngine::Monster();
		});
		return true;
	}();
}