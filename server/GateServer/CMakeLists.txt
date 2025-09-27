cmake_minimum_required(VERSION 3.16)

project(gateServer LANGUAGES CXX)

# 添加CMake策略设置
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

set(VCPKG_TARGET_TRIPLET "arm64-osx" CACHE STRING "")
set(CONFIG_DEST_DIR "$<TARGET_FILE_DIR:gateserver>")

# 查找所需的包
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(jsoncpp CONFIG REQUIRED PATHS "${CMAKE_TOOLCHAIN_FILE}/../installed/arm64-osx/share/jsoncpp")
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# 添加编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")

file(GLOB PROTO_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/*.pb.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/*.grpc.pb.cc"
)

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
    src/VarifyGrpcClient.h
    src/VarifyGrpcClient.cc
    ${PROTO_SRCS}
    # ${GRPC_SRCS}
)

# 链接依赖库
target_link_libraries(gateserver PRIVATE 
    Boost::system
    Boost::thread
    JsonCpp::JsonCpp
    
    gRPC::grpc++
    protobuf::libprotobuf
)

target_include_directories(gateserver PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${VCPKG_INSTALLED_DIR}/arm64-osx/include
    # ${PROTO_OUTPUT_DIR} 
    # ${PROTOBUF_INCLUDE_DIRS} 
)

add_custom_command(TARGET gateserver POST_BUILD
    # COMMAND ${CMAKE_COMMAND} -E make_directory "${CONFIG_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config.ini"
        "${CONFIG_DEST_DIR}/config.ini"
    COMMENT "Copying config.ini to output directory"
)
