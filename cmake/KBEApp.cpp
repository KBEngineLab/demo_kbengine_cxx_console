#include "KBEApp.h"

KBEApp::KBEApp()
{
    installEvents();
}

void KBEApp::installEvents()
{
    KBENGINE_REGISTER_EVENT_IN(KBEngine::KBEventTypes::onScriptVersionNotMatch, onScriptVersionNotMatch);
    KBENGINE_REGISTER_EVENT_IN(KBEngine::KBEventTypes::onVersionNotMatch, onVersionNotMatch);
}

void KBEApp::deregisterEvents()
{
}

void KBEApp::onVersionNotMatch(std::shared_ptr<UKBEventData> pEventData)
{
    printf("KBEApp::onVersionNotMatch: clientVersion=%s, serverVersion=%s\n",
        static_cast<const UKBEventData_onVersionNotMatch&>(*pEventData).clientVersion.c_str(),
		static_cast<const UKBEventData_onVersionNotMatch&>(*pEventData).serverVersion.c_str());
}

void KBEApp::onScriptVersionNotMatch(std::shared_ptr<UKBEventData> pEventData)
{
    printf("KBEApp::onScriptVersionNotMatch: clientScriptVersion=%s, serverScriptVersion=%s\n",
        static_cast<const UKBEventData_onScriptVersionNotMatch&>(*pEventData).clientScriptVersion.c_str(),
		static_cast<const UKBEventData_onScriptVersionNotMatch&>(*pEventData).serverScriptVersion.c_str());
}



