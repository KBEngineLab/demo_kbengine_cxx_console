#include "Gate.h"
#include "../kbe_cxx_plugins/Entity.h"
#include "../kbe_cxx_plugins/EntityFactory.h"

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
	const bool registeredGate = []() {
		EntityFactory::instance().registerType("Gate", []() {
			return new KBEngine::Gate();
		});
		return true;
	}();
}