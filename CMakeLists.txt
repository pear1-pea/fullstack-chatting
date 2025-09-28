cmake_minimum_required(VERSION 3.16)
project(fullstack-chatting)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if(NOT CMAKE_BUILD_TYPE) set(CMAKE_BUILD_TYPE Debug) endif()
# set(CMAKE_PREFIX_PATH "/Users/lizengxin/Qt/6.8.2/macos/lib/cmake")

if(DEFINED ENV{VCPKG_ROOT})
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")
endif()

# if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64") set(VCPKG_TARGET_TRIPLET
# "arm64-osx" CACHE STRING "") endif()

add_subdirectory(client)
add_subdirectory(server)

# TODO build general options add_custom_target(all_projects)
# add_dependencies(all_projects fullstack-chatting gateserver)
# option(BUILD_CLIENT "Build Qt client" ON) option(BUILD_SERVER "Build gRPC
# server" ON)

# if(BUILD_CLIENT) add_subdirectory(client) endif()

# if(BUILD_SERVER) add_subdirectory(server) endif()
