
#include <iostream>

#include "Avatar.h"
#include "UdpClient.h"
#include "htime.h"
#include "kbengine_cxx_plugins/AnyObject.h"
#include "kbengine_cxx_plugins/AvatarBase.h"
#include "kbengine_cxx_plugins/GameThreadDispatcher.h"
#include "kbengine_cxx_plugins/KBECommon.h"
#include "kbengine_cxx_plugins/KBEngine.h"
#include "kbengine_cxx_plugins/KBEngineArgs.h"
#include "kbengine_cxx_plugins/KBEvent.h"



void onVersionNotMatch(const UKBEventData* pEventData)
{
}

void onScriptVersionNotMatch(const UKBEventData* pEventData)
{
}

void installEvents()
{
    // KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onScriptVersionNotMatch, onScriptVersionNotMatch);
    // KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onImportClientSDKSuccessfully, onImportClientSDKSuccessfully);
    // KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onDownloadSDK, onDownloadSDK);
}


bool login(KBString username, KBString password, KBArray<uint8> datas)
{
    if (!KBEngine::KBEngineApp::getSingleton().isInitialized())
    {
        return false;
    }

    KBEngine::KBEngineApp::getSingleton().reset();

    auto pEventData = std::make_shared<UKBEventData_login>();
    pEventData->username = username;
    pEventData->password = password;
    pEventData->datas = datas;
    KBENGINE_EVENT_FIRE(KBEngine::KBEventTypes::login, pEventData);
    return true;
}


struct Actor {
    std::string name;
    Actor(const std::string& n) : name(n) {}
    void speak() const { printf("I am %s\n", name.c_str()); }
};

// 在 main() 或程序初始化处
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif


    printf("测试中文: %s\n", "服务器没有准备好");
    ERROR_MSG("测试中文: %s", "服务器没有准备好");

    AnyObject a = 42;   // 存int，小对象，SBO内存
    printf("a = %d\n", a.get<int>());

    a = std::string("hello world"); // 若字符串长度较小，也会用 SBO
    printf("%s\n", a.get<std::string>().c_str());


    // UE 风格：原生 Actor*
    Actor* actor = new Actor("RawActor");
    a = actor;
    a.get<Actor*>()->speak();

    // shared_ptr

    // unique_ptr（移动）
    std::unique_ptr<Actor> up(new Actor("UP"));
    a = std::move(up);
    a.get<std::unique_ptr<Actor>>()->speak();


    std::shared_ptr<hv::EventLoopThread> mainLoopThread_ = std::make_shared<hv::EventLoopThread>();
    mainLoopThread_->start(); // 内部会创建 EventLoop 并在新线程 run()

    hv::EventLoopPtr mainLoop_ = mainLoopThread_->loop(); // 获取 EventLoopPtr

    mainLoop_->setInterval(15, [](hv::TimerID timerID) {
        // this->process();
        GameThreadDispatcher::Instance().Pump();

        if (KBEngine::KBEngineApp::getSingleton().isInitialized()) {
            KBEngine::KBEngineApp::getSingleton().process();
        }
    });

    KBString ip = KBTEXT("127.0.0.1");
    int port = 20013;
    int syncPlayerMS = 1000 / 10;
    bool useAliasEntityID = true;
    bool isOnInitCallPropertysSetMethods = true;
    bool forceDisableUDP = true;
    EKCLIENT_TYPE clientType = EKCLIENT_TYPE::CLIENT_TYPE_WIN;
    NETWORK_ENCRYPT_TYPE networkEncryptType = NETWORK_ENCRYPT_TYPE::ENCRYPT_TYPE_NONE;
    int serverHeartbeatTick = 10;
    int TCP_SEND_BUFFER_MAX = TCP_PACKET_MAX;
    int TCP_RECV_BUFFER_MAX = TCP_PACKET_MAX;
    int UDP_SEND_BUFFER_MAX = 128;
    int UDP_RECV_BUFFER_MAX = 128;



    auto* pArgs = new KBEngine::KBEngineArgs();
    pArgs->ip = ip;
    pArgs->port = port;
    pArgs->syncPlayerMS = syncPlayerMS;
    pArgs->useAliasEntityID = useAliasEntityID;
    pArgs->isOnInitCallPropertysSetMethods = isOnInitCallPropertysSetMethods;
    pArgs->forceDisableUDP = forceDisableUDP;
    pArgs->clientType = clientType;
    pArgs->networkEncryptType = networkEncryptType;
    pArgs->serverHeartbeatTick = serverHeartbeatTick / 2;
    pArgs->TCP_SEND_BUFFER_MAX = TCP_SEND_BUFFER_MAX;
    pArgs->TCP_RECV_BUFFER_MAX = TCP_RECV_BUFFER_MAX;
    pArgs->UDP_SEND_BUFFER_MAX = UDP_SEND_BUFFER_MAX;
    pArgs->UDP_RECV_BUFFER_MAX = UDP_RECV_BUFFER_MAX;
    pArgs->disableMainLoop = true;

    KBEngine::KBEngineApp::getSingleton().initialize(pArgs);


    login("123","123",KBArray<uint8>());



    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "close") {
            KBEngine::KBEngineApp::getSingleton().destroy();
        }else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}

