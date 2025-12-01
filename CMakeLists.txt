cmake_minimum_required(VERSION 3.10)
project(cxx_sdk_demo)

# 使用 C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/utf-8")
endif()

# 定义 Windows 宏
add_definitions(-D_WIN32 -D_HV_WINDOWS_ -DWIN32_LEAN_AND_MEAN)
add_definitions(-DWITH_KCP=1)


# include 路径
include_directories(
        ${CMAKE_SOURCE_DIR}/kbengine_cxx_plugins/libhv
        ${CMAKE_SOURCE_DIR}/kbengine_cxx_plugins/libhv/include/hv
        ${CMAKE_SOURCE_DIR}/kbe_scripts
)

# lib 路径
link_directories(
        ${CMAKE_SOURCE_DIR}/kbengine_cxx_plugins/libhv/lib/win/Release/
)

# ------------ 自动收集源文件 ------------
file(GLOB_RECURSE KBE_SDK_SOURCES
        ${CMAKE_SOURCE_DIR}/kbengine_cxx_plugins/*.cpp
        ${CMAKE_SOURCE_DIR}/kbengine_cxx_plugins/*.c
        ${CMAKE_SOURCE_DIR}/kbengine_cxx_plugins/*.h
)

file(GLOB_RECURSE KBE_SCRIPT_SOURCES
        ${CMAKE_SOURCE_DIR}/kbe_scripts/*.cpp
        ${CMAKE_SOURCE_DIR}/kbe_scripts/*.h
)

# 主程序
add_executable(cxx_sdk_demo
        main.cpp
        ${KBE_SDK_SOURCES}
        ${KBE_SCRIPT_SOURCES}
)

# ------------ 链接 libhv 库 ------------
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(cxx_sdk_demo hv_d)
else()
    target_link_libraries(cxx_sdk_demo hv)
endif()

# ------------ 自动复制 hv.dll ------------
add_custom_command(TARGET cxx_sdk_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/kbengine_cxx_plugins/libhv/lib/win/Debug/hv.dll"
        $<TARGET_FILE_DIR:cxx_sdk_demo>
)
