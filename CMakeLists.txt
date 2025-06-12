cmake_minimum_required(VERSION 3.16)
project(fullstack-chatting)

# 设置全局的 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_PREFIX_PATH "/Users/lizengxin/Qt/6.8.2/macos/lib/cmake")

# 如果存在 vcpkg，设置 toolchain 文件
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# 设置 vcpkg triplet（如果在 macOS ARM 上）
if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    set(VCPKG_TARGET_TRIPLET "arm64-osx" CACHE STRING "")
endif()

# 添加子项目
add_subdirectory(client)
add_subdirectory(server)
