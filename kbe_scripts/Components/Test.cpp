#include "Test.h"
#include "../Avatar.h"
#include "../../kbengine_cxx_plugins/EntityComponentFactory.h"

namespace KBEngine
{

Test::Test():
	TestBase()
{
}

Test::~Test()
{
}

void Test::onAttached(Entity* pOwnerEntity)
{
	DEBUG_MSG("Test::onAttached: entity(%d)!", pOwnerEntity->id());
}

void Test::onDetached(Entity* pOwnerEntity)
{
	DEBUG_MSG("Test::onDetached: entity(%d)!", pOwnerEntity->id());
}

void Test::onOwnChanged(int32 oldValue) {
	DEBUG_MSG("Test::onOwnChanged: own(%d)!", oldValue);
}

void Test::onStateChanged(int32 oldValue) {
	DEBUG_MSG("Test::onOwnChanged: state(%d)!", oldValue);
}

void Test::helloCB(int32 arg1)
{
	DEBUG_MSG("Test::helloCB: arg1(%d)!", arg1);
	//((Avatar*)pOwner)->pCellEntityCall->component1->hello(795);
}

}


// 静态注册
namespace {
	const bool registered = []() {
		EntityComponentFactory::instance().registerType("Test", []() {
			return new KBEngine::Test();
		});
		return true;
	}();
}