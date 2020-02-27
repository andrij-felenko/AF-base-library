cmake_minimum_required(VERSION 3.14)
project(lib_baseAF VERSION 0.0.1)

set (baseAF_h
#    include/lib_baseAF/applicationAF.h
#    include/lib_baseAF/crypt.h
    include/lib_baseAF/afDir.h
    include/lib_baseAF/afEnum.h
    include/lib_baseAF/afFile.h
    include/lib_baseAF/afFunction.h
    include/lib_baseAF/afId.h
    include/lib_baseAF/afIdAccount.h
    include/lib_baseAF/afIdBit.h
    include/lib_baseAF/afIdHistory.h
    include/lib_baseAF/afIdInfo.h
    include/lib_baseAF/afIdOperate.h
    include/lib_baseAF/afIdObject.h
    include/lib_baseAF/afIdObjectBit.h
#    include/lib_baseAF/afLibHandler.h
    include/lib_baseAF/afLibSingle.h
    include/lib_baseAF/afPlugin.h
#    include/lib_baseAF/afPluginHandler.h
    include/lib_baseAF/afPluginMetadata.h
#    include/lib_baseAF/afPluginSingle.h
    include/lib_baseAF/afPreferences.h
    include/lib_baseAF/afSetting.h
    include/lib_baseAF/afStorage.h
    include/lib_baseAF/afSystem.h
    include/lib_baseAF/afVersion.h
)

set (baseAF_cpp
    base/afEnum.cpp
    base/afFunction.cpp
    base/afIdAccount.cpp
    base/afIdHistory.cpp
    base/afIdInfo.cpp
    base/afIdOperate.cpp
    base/afIdObject.cpp
    base/afIdObjectBit.cpp
    base/afSystem.cpp
#    loader/afLibHandler.cpp
    loader/afLibSingle.cpp
    loader/afPlugin.cpp
#    loader/afPluginHandler.cpp
    loader/afPluginMetadata.cpp
#    loader/afPluginSingle.cpp
    loader/afVersion.cpp
#    tool/afCrypt.cpp
    tool/afDir.cpp
    tool/afFile.cpp
    tool/afPreferences.cpp
    tool/afSetting.cpp
    tool/afStorage.cpp
)

set (CMAKE_AUTOMOC, true)
find_package(Qt5Core)

add_library(lib_baseAF ${baseAF_h} ${baseAF_cpp})

set_target_properties(lib_baseAF PROPERTIES OUTPUT_NAME "baseAF")
target_link_libraries(lib_baseAF Qt5::Core)
target_include_directories(lib_baseAF
    PUBLIC  include
    PRIVATE include/lib_baseAF
)
