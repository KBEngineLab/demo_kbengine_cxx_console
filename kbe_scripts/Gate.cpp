#include "Gate.h"
#include "../kbengine_cxx_plugins/Entity.h"
#include "LogicEvents.h"
#include "../kbengine_cxx_plugins/EntityFactory.h"

namespace KBEngine
{

Gate::Gate():
	GateBase()
{
}

Gate::~Gate()
{
}

void Gate::__init__()
{
}



}



// 静态注册
namespace {
	const bool registered = []() {
		EntityFactory::instance().registerType("Gate", []() {
			return new KBEngine::Gate();
		});
		return true;
	}();
}