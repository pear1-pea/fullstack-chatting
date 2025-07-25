cmake_minimum_required(VERSION 3.16)

project(fullstack-chatting VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

# 自动收集 src 目录下所有源码文件
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.h
    src/*.ui
)

set(MACOSX_BUNDLE_ICON_FILE head.icns)
set_source_files_properties(${CMAKE_SOURCE_DIR}/res/head.icns
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(fullstack-chatting
        MANUAL_FINALIZATION
        MACOSX_BUNDLE
        ${PROJECT_SOURCES}
        res/rc.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET fullstack-chatting APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(fullstack-chatting SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(fullstack-chatting
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_sources(fullstack-chatting PRIVATE ${CMAKE_SOURCE_DIR}/res/head.icns)

target_link_libraries(fullstack-chatting
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
)

# 配置文件拷贝
if(APPLE)
    set(CONFIG_DEST_DIR "$<TARGET_FILE_DIR:fullstack-chatting>/../Resources")
else()
    set(CONFIG_DEST_DIR "$<TARGET_FILE_DIR:fullstack-chatting>")
endif()

add_custom_command(TARGET fullstack-chatting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CONFIG_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/config.ini"
        "${CONFIG_DEST_DIR}/config.ini"
    COMMENT "Copying config.ini to output directory"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.fullstack-chatting)
endif()
set_target_properties(fullstack-chatting PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS fullstack-chatting
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(fullstack-chatting)
endif()
