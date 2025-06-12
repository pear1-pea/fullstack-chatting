cmake_minimum_required(VERSION 3.20)

# 添加CMake策略设置
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

set(CMAKE_TOOLCHAIN_FILE "/Users/lizengxin/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(gateserver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VCPKG_TARGET_TRIPLET "arm64-osx" CACHE STRING "")

# 查找所需的包
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(jsoncpp CONFIG REQUIRED)

# 添加编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")

# Add compile definitions for Boost Beast
add_definitions(-DBOOST_BEAST_USE_STD_STRING_VIEW)

# GateServer 项目配置
add_executable(gateserver 
    main.cpp
    CServer.cpp
    CServer.h
    const.h
    HttpConnection.h
    HttpConnection.cpp
    LogicSystem.cpp
    LogicSystem.h
    Singleton.h
)

# 链接依赖库
target_link_libraries(gateserver PRIVATE 
    Boost::thread
    JsonCpp::JsonCpp
)

target_include_directories(gateserver PRIVATE ${Boost_INCLUDE_DIRS})