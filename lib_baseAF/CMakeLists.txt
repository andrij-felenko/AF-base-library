cmake_minimum_required(VERSION 3.14)
project(lib_baseAF VERSION 0.0.1)

set (baseAF_h
#    include/lib_baseAF/applicationAF.h
#    include/lib_baseAF/crypt.h
    include/lib_baseAF/dir.h
    include/lib_baseAF/enumAF.h
    include/lib_baseAF/file.h
    include/lib_baseAF/function.h
    include/lib_baseAF/id.h
#    include/lib_baseAF/libHandler.h
    include/lib_baseAF/libSingle.h
    include/lib_baseAF/plugin.h
#    include/lib_baseAF/pluginHandler.h
    include/lib_baseAF/pluginMetadata.h
#    include/lib_baseAF/pluginSingle.h
    include/lib_baseAF/preferences.h
    include/lib_baseAF/setting.h
    include/lib_baseAF/storage.h
    include/lib_baseAF/system.h
    include/lib_baseAF/version.h
)

set (baseAF_cpp
    base/enumAF.cpp
    base/function.cpp
    base/id.cpp
    base/system.cpp
#    loader/libHandler.cpp
    loader/libSingle.cpp
    loader/plugin.cpp
#    loader/pluginHandler.cpp
    loader/pluginMetadata.cpp
#    loader/pluginSingle.cpp
    loader/version.cpp
#    tool/crypt.cpp
    tool/dir.cpp
    tool/file.cpp
    tool/preferences.cpp
    tool/setting.cpp
    tool/storage.cpp
)

set (CMAKE_AUTORCC)
find_package(Qt5Core)

add_library(lib_baseAF ${baseAF_h} ${baseAF_cpp})

set_target_properties(lib_baseAF PROPERTIES OUTPUT_NAME "baseAF")
target_link_libraries(lib_baseAF Qt5::Core)
target_include_directories(lib_baseAF
    PUBLIC  include
    PRIVATE include/lib_baseAF
)
