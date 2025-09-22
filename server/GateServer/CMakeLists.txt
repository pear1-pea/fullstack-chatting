cmake_minimum_required(VERSION 3.16)

project(gateServer LANGUAGES CXX)

# 添加CMake策略设置
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()


# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VCPKG_TARGET_TRIPLET "arm64-osx" CACHE STRING "")

# 查找所需的包
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(jsoncpp CONFIG REQUIRED PATHS "${CMAKE_TOOLCHAIN_FILE}/../installed/arm64-osx/share/jsoncpp")
# 添加编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")

# Add compile definitions for Boost Beast
add_definitions(-DBOOST_BEAST_USE_STD_STRING_VIEW)

# GateServer 项目配置
add_executable(gateserver 
    src/GateServer.cpp
    src/CServer.cpp
    src/CServer.h
    src/const.h
    src/HttpConnection.h
    src/HttpConnection.cpp
    src/LogicSystem.cpp
    src/LogicSystem.h
    src/Singleton.h
)

# 链接依赖库
target_link_libraries(gateserver PRIVATE 
    Boost::system
    Boost::thread
    JsonCpp::JsonCpp
)

target_include_directories(gateserver PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${VCPKG_INSTALLED_DIR}/arm64-osx/include
)