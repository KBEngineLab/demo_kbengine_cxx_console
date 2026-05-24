#pragma once

#include "kbe_cxx_plugins/KBEMain.h"
class KBEApp : public KBEMain
{
public:
    KBEApp();
    ~KBEApp();


protected:
    virtual void installEvents();
    virtual void deregisterEvents();

private:
    void onVersionNotMatch(std::shared_ptr<UKBEventData> pEventData);
    void onScriptVersionNotMatch(std::shared_ptr<UKBEventData> pEventData);
};

