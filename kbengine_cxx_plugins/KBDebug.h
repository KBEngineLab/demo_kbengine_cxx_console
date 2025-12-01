#pragma once

// =========================================
// 平台判断
// =========================================

#if defined(__UNREAL__) || defined(UE_BUILD_DEBUG) || defined(UE_SERVER) || defined(UE_CLIENT)
    #define KBE_PLATFORM_UE 1
#elif defined(CC_TARGET_PLATFORM)
    #define KBE_PLATFORM_COCOS 1
#else
    #define KBE_PLATFORM_CPP 1
#endif

// =========================================
// UE 版本
// =========================================
#if KBE_PLATFORM_UE

#include "CoreMinimal.h"
#include "Misc/OutputDevice.h"
#include "Engine/Engine.h"

#define INFO_MSG(Format, ...) \
{ \
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    UE_LOG(LogTemp, Log, TEXT("%s"), *Msg); \
}

#define DEBUG_MSG(Format, ...) \
{ \
    const FString Msg = FString::Printf(TEXT("**DEBUG** " Format), ##__VA_ARGS__); \
    UE_LOG(LogTemp, Log, TEXT("%s"), *Msg); \
}

#define WARNING_MSG(Format, ...) \
{ \
    const FString Msg = FString::Printf(TEXT("**WARNING** " Format), ##__VA_ARGS__); \
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg); \
}

#define ERROR_MSG(Format, ...) \
{ \
    const FString Msg = FString::Printf(TEXT("**ERROR** " Format), ##__VA_ARGS__); \
    UE_LOG(LogTemp, Error, TEXT("%s"), *Msg); \
}

#define SCREEN_WARNING_MSG(Format, ...) \
{ \
    const FString Msg = FString::Printf(TEXT("**WARNING** " Format), ##__VA_ARGS__); \
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg); \
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Msg); \
}

#define SCREEN_ERROR_MSG(Format, ...) \
{ \
    const FString Msg = FString::Printf(TEXT("**ERROR** " Format), ##__VA_ARGS__); \
    UE_LOG(LogTemp, Error, TEXT("%s"), *Msg); \
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Msg); \
}

#define SCREENDEBUG(Format, ...) \
{ \
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, Msg); \
}

#endif // UE



// =========================================
// Cocos2d-x 版本
// =========================================
#if KBE_PLATFORM_COCOS

#include "base/CCConsole.h"
#include "base/CCDirector.h"
#include "base/CCScheduler.h"
#include <string>

#define INFO_MSG(Format, ...) \
{ \
    cocos2d::log("[INFO] " Format, ##__VA_ARGS__); \
}

#define DEBUG_MSG(Format, ...) \
{ \
    cocos2d::log("[DEBUG] " Format, ##__VA_ARGS__); \
}

#define WARNING_MSG(Format, ...) \
{ \
    cocos2d::log("[WARNING] " Format, ##__VA_ARGS__); \
}

#define ERROR_MSG(Format, ...) \
{ \
    cocos2d::log("[ERROR] " Format, ##__VA_ARGS__); \
}

#define SCREEN_WARNING_MSG(Format, ...) \
{ \
    cocos2d::log("[WARNING] " Format, ##__VA_ARGS__); \
    auto msg = cocos2d::StringUtils::format("WARNING: " Format, ##__VA_ARGS__); \
    cocos2d::MessageBox(msg.c_str(), "Warning"); \
}

#define SCREEN_ERROR_MSG(Format, ...) \
{ \
    cocos2d::log("[ERROR] " Format, ##__VA_ARGS__); \
    auto msg = cocos2d::StringUtils::format("ERROR: " Format, ##__VA_ARGS__); \
    cocos2d::MessageBox(msg.c_str(), "Error"); \
}

#define SCREENDEBUG(Format, ...) \
{ \
    cocos2d::log("[SCREEN] " Format, ##__VA_ARGS__); \
}

#endif // Cocos



// =========================================
// 原生 C++ 版本
// =========================================
#if KBE_PLATFORM_CPP

#include <cstdio>
#include <cstdarg>
//
// inline void kbe_printf(const char* prefix, const char* fmt, ...) {
//     char buffer[2048];
//     va_list args;
//     va_start(args, fmt);
//     vsnprintf(buffer, sizeof(buffer), fmt, args);
//     va_end(args);
//
// #ifdef _WIN32
//     // 转 GBK 输出
//     int size_needed = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, nullptr, 0);
//     std::wstring wbuf(size_needed, L'\0');
//     MultiByteToWideChar(CP_UTF8, 0, buffer, -1, &wbuf[0], size_needed);
//
//     // 输出到控制台
//     DWORD written;
//     WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wbuf.c_str(), (DWORD)wbuf.size()-1, &written, nullptr);
//     WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, &written, nullptr);
// #else
//     printf("%s %s\n", prefix, buffer);
// #endif
// }

inline void kbe_printf(const char* prefix, const char* fmt, ...)
{
    char buffer[2048];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    printf("%s %s\n", prefix, buffer);
}

#define INFO_MSG(Format, ...) \
    kbe_printf("[INFO]", Format, ##__VA_ARGS__);

#define DEBUG_MSG(Format, ...) \
    kbe_printf("[DEBUG]", Format, ##__VA_ARGS__);

#define WARNING_MSG(Format, ...) \
    kbe_printf("[WARNING]", Format, ##__VA_ARGS__);

#define ERROR_MSG(Format, ...) \
    kbe_printf("[ERROR]", Format, ##__VA_ARGS__);

#define SCREEN_WARNING_MSG(Format, ...) \
    kbe_printf("[WARNING][SCREEN]", Format, ##__VA_ARGS__);

#define SCREEN_ERROR_MSG(Format, ...) \
    kbe_printf("[ERROR][SCREEN]", Format, ##__VA_ARGS__);

#define SCREENDEBUG(Format, ...) \
    kbe_printf("[SCREEN]", Format, ##__VA_ARGS__);

#endif // 原生 C++

