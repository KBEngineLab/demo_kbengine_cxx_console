#include "NPC.h"
#include "../kbengine_cxx_plugins/Entity.h"
#include "LogicEvents.h"

#include "../kbengine_cxx_plugins/EntityFactory.h"

namespace KBEngine
{

NPC::NPC():
	NPCBase()
{
}

NPC::~NPC()
{
}

void NPC::__init__()
{
}



}


// 静态注册
namespace {
	const bool registered = []() {
		EntityFactory::instance().registerType("NPC", []() {
			return std::make_unique<KBEngine::NPC>();
		});
		return true;
	}();
}