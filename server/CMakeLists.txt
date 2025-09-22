cmake_minimum_required(VERSION 3.16)
project(Server)
# 服务器端共同的编译选项
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# macOS ARM 架构的编译选项
# if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")
# endif()

# Boost Beast 的定义
# add_definitions(-DBOOST_BEAST_USE_STD_STRING_VIEW)

# 查找公共依赖
# find_package(Boost REQUIRED COMPONENTS system thread)
# find_package(jsoncpp CONFIG REQUIRED)

# find_package(Protobuf CONFIG REQUIRED)
# find_package(gRPC CONFIG REQUIRED)
# TODO:添加 protobuf 相关的配置
# find_package(Protobuf REQUIRED)
# 处理 proto 文件的生成等

# 添加服务器子项目
add_subdirectory(GateServer)
# 未来可以添加其他服务
# add_subdirectory(VarifyServer) 