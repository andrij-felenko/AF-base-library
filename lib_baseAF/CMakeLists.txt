cmake_minimum_required(VERSION 3.14)
project(lib_baseAF VERSION 0.0.1)

set (baseAF_h
    include/afBaseLib.h
    include/lib_baseAF/afDir.h
    include/lib_baseAF/afEnum.h
    include/lib_baseAF/afFile.h
    include/lib_baseAF/afFunction.h
    include/lib_baseAF/afId.h
    include/lib_baseAF/afIdAccount.h
    include/lib_baseAF/afIdBit.h
    include/lib_baseAF/afIdHistory.h
    include/lib_baseAF/afIdOperate.h
    include/lib_baseAF/afIdObject.h
    include/lib_baseAF/afIdObjectBit.h
    include/lib_baseAF/afPreference.h
    include/lib_baseAF/afSetting.h
    include/lib_baseAF/afStorage.h
    include/lib_baseAF/afSystem.h
    include/lib_baseAF/afTransferOperateList.h
)

set (baseAF_cpp
    base/afEnum.cpp
    base/afFunction.cpp
    base/afIdAccount.cpp
    base/afIdHistory.cpp
    base/afIdOperate.cpp
    base/afIdObject.cpp
    base/afIdObjectBit.cpp
    base/afSystem.cpp
    tool/afDir.cpp
    tool/afFile.cpp
    tool/afPreference.cpp
    tool/afSetting.cpp
    tool/afStorage.cpp
    tool/afTransferOperateList.cpp
)

set (initialize_header
    include/AFbaseLib
    include/AFbase/AfEnum
    include/AFbase/AfFile
    include/AFbase/AfFunction
    include/AFbase/AfId
    include/AFbase/AfIdHistory
    include/AFbase/AfIdObject
    include/AFbase/AfIdObjectBit
    include/AFbase/AfIdOperate
    include/AFbase/AfPreference
    include/AFbase/AfSetting
    include/AFbase/AfSystem
    include/AFbase/AfTransferOperateList
)

set (CMAKE_AUTOMOC, true)
find_package(Qt5Core)

add_library(lib_baseAF ${baseAF_h} ${baseAF_cpp})

set_target_properties(lib_baseAF PROPERTIES OUTPUT_NAME "baseAF")
target_link_libraries(lib_baseAF Qt5::Core)
target_include_directories(lib_baseAF
    PUBLIC  include
    PUBLIC  include/AFbase
    PRIVATE include/lib_baseAF
)
