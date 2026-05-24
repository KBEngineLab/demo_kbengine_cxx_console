
#include <iostream>

#include "kbe_cxx_plugins/KBEngine.h"
#include "KBEApp.h"



int main(int argc, char* argv[]) {


    auto* app = new KBEApp();
    app->ip = "127.0.0.1";
    app->port = 20013;
    app->disableMainLoop = false;

    app->init();

    app->login("123", "123", KBArray<uint8>());

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "close") {
            KBEngine::KBEngineApp::getSingleton().destroy();
        }
        else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}