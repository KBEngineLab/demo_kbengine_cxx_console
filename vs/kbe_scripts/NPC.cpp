#include "NPC.h"

#include "../kbe_cxx_plugins/Entity.h"
#include "LogicEvents.h"
#include "../kbe_cxx_plugins/EntityFactory.h"

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

void NPC::onDestroy()
{
	NPCBase::onDestroy();

}

void NPC::onEnterWorld()
{
	NPCBase::onEnterWorld();
	
}

void NPC::onLeaveWorld()
{
	NPCBase::onLeaveWorld();
	
}

void NPC::onEnterSpace()
{
	NPCBase::onEnterSpace();

}

void NPC::onLeaveSpace()
{
	NPCBase::onLeaveSpace();
	
}


}


// 静态注册
namespace {
	const bool registeredNPC = []() {
		EntityFactory::instance().registerType("NPC", []() {
			return new KBEngine::NPC();
		});
		return true;
	}();
}