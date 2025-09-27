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
set(CONFIG_DEST_DIR "$<TARGET_FILE_DIR:gateserver>")

# 查找所需的包
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(jsoncpp CONFIG REQUIRED PATHS "${CMAKE_TOOLCHAIN_FILE}/../installed/arm64-osx/share/jsoncpp")

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)




# 添加编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")





# 定义 Protobuf 文件和输出目录
set(PROTOBUF_FILE_NAME "message.proto")
set(PROTOBUF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src) # .proto 文件所在的目录
set(PROTO_SRC_FILE ${PROTOBUF_DIR}/${PROTOBUF_FILE_NAME})
set(PROTO_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto_gen) # 编译生成的 .pb.cc 和 .pb.h 的临时存放目录




# Add compile definitions for Boost Beast
add_definitions(-DBOOST_BEAST_USE_STD_STRING_VIEW)




# 设置编译标志，-I 指定 .proto 文件的搜索路径
set(PROTO_FLAGS -I${PROTOBUF_DIR})

# 创建输出目录
file(MAKE_DIRECTORY ${PROTO_OUTPUT_DIR})

# 定义生成的源文件和头文件的完整路径
set(PROTO_GENERATED_SRC ${PROTO_OUTPUT_DIR}/message.pb.cc)
set(PROTO_GENERATED_HDR ${PROTO_OUTPUT_DIR}/message.pb.h)
set(GRPC_GENERATED_SRC ${PROTO_OUTPUT_DIR}/message.grpc.pb.cc)
set(GRPC_GENERATED_HDR ${PROTO_OUTPUT_DIR}/message.grpc.pb.h)
# 执行 Protobuf 编译命令
# COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_OUTPUT_DIR} ${PROTO_SRC_FILE}
# 使用 add_custom_command 来确保每次构建时都会检查 .proto 文件是否有更新
add_custom_command(
    OUTPUT ${PROTO_GENERATED_SRC} ${PROTO_GENERATED_HDR} ${GRPC_GENERATED_SRC} ${GRPC_GENERATED_HDR}
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS 
        ${PROTO_FLAGS} 
        --cpp_out=${PROTO_OUTPUT_DIR} 
        --grpc_out=${PROTO_OUTPUT_DIR}
        --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE}
        ${PROTO_SRC_FILE}
    DEPENDS ${PROTO_SRC_FILE}
    VERBATIM
)

# 将生成的源文件添加到项目编译列表中
set(PROTO_SRCS ${PROTO_GENERATED_SRC} ${GRPC_GENERATED_SRC})




# # 找到所有需要编译的proto文件
# file(GLOB protobuf_files
#     fullstack-chatting/server/GateServer/src/*.proto
# )

# # 定义编译之后生成文件的目录
# SET(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
# LIST(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR})

# 设置好之后还要依次编译每个文件，并将生成的移回原始目录
# FOREACH(FIL ${protobuf_files})

#     GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE
    
#     string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FILE})
#     string(REGEX REPLACE "(.+)\\${FILE_NAME}.*" "\\1" FILE_PATH ${FIL})

#     string(REGEX MATCH "(/src/)")
# ENDFOREACH()









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
    ${PROTO_OUTPUT_DIR} 
    ${PROTOBUF_INCLUDE_DIRS} 
)

add_custom_command(TARGET gateserver POST_BUILD
    # COMMAND ${CMAKE_COMMAND} -E make_directory "${CONFIG_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config.ini"
        "${CONFIG_DEST_DIR}/config.ini"
    COMMENT "Copying config.ini to output directory"
)
