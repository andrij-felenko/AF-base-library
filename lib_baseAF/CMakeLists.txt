cmake_minimum_required(VERSION 3.14)
project(lib_baseAF VERSION 0.0.1)

set (baseAF_h
    include/applicationAF.h
    include/dir.h
    include/enum.h
    include/file.h
    include/function.h
    include/libHandler.h
    include/libSingle.h
    include/origin.h
    include/plugin.h
    include/pluginHandler.h
    include/pluginMetadata.h
    include/pluginSingle.h
    include/setting.h
    include/system.h
    include/user.h
    include/userHandler.h
    include/version.h
)

set (baseAF_cpp
    control/dir.cpp
    control/origin.cpp
    control/setting.cpp
    loaded/libHandler.cpp
    loaded/libSingle.cpp
    loaded/plugin.cpp
    loaded/pluginHandler.cpp
    loaded/pluginMetadata.cpp
    loaded/pluginSingle.cpp
    loaded/version.cpp
    tool/enum.cpp
    tool/file.cpp
    tool/function.cpp
    tool/system.cpp
    user/user.cpp
    user/userHandler.cpp
)

set (CMAKE_AUTORCC)
find_package(Qt5Core)

add_library(lib_baseAF SHARED ${baseAF_h} ${baseAF_cpp})

set_target_properties(lib_baseAF PROPERTIES OUTPUT_NAME "baseAF")
target_include_directories(lib_baseAF PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(lib_baseAF Qt5::Core)
# TODO cmake target properties, how it work?
#set_target_properties(lib_baseAF PROPERTIES PUBLIC_HEADER ${baseAF_h})
