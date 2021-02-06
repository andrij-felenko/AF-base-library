cmake_minimum_required(VERSION 3.19)
project(lib_baseAF VERSION 0.0.1)

set (baseAF_h
    include/afBaseLib.h
    include/afAccountLib.h
    include/lib_baseAF/afAccount.h
    include/lib_baseAF/afAccountGroup.h
    include/lib_baseAF/afAccountInfo.h
    include/lib_baseAF/afAccountStorage.h
    include/lib_baseAF/afAPI.h
    include/lib_baseAF/afDir.h
    include/lib_baseAF/afEnum.h
    include/lib_baseAF/afFile.h
    include/lib_baseAF/afFunction.h
    include/lib_baseAF/afId.h
    include/lib_baseAF/afIdAccount.h
    include/lib_baseAF/afIdBit.h
    include/lib_baseAF/afIdGlobalBit.h
    include/lib_baseAF/afIdHistory.h
    include/lib_baseAF/afIdOperate.h
    include/lib_baseAF/afIdObject.h
    include/lib_baseAF/afIdObjectTemplate.h
    include/lib_baseAF/afIdObjectBit.h
    include/lib_baseAF/afPreference.h
    include/lib_baseAF/afServerSettings.h
    include/lib_baseAF/afSetting.h
    include/lib_baseAF/afStorage.h
    include/lib_baseAF/afSystem.h
    include/lib_baseAF/afTransferOperateList.h
    include/lib_baseAF/afLinkedList.h
    )

set (baseAF_cpp
    account/afAccount.cpp
    account/afAccountGroup.cpp
    account/afAccountInfo.cpp
    account/afAccountStorage.cpp
    base/afAPI.cpp
    base/afEnum.cpp
    base/afFunction.cpp
    base/afIdAccount.cpp
    base/afIdGlobalBit.cpp
    base/afIdHistory.cpp
    base/afIdOperate.cpp
    base/afIdObject.cpp
    base/afIdObjectTemplate.cpp
    base/afIdObjectBit.cpp
    base/afSystem.cpp
    tool/afDir.cpp
    tool/afFile.cpp
    tool/afPreference.cpp
    tool/afServerSettings.cpp
    tool/afSetting.cpp
    tool/afStorage.cpp
    tool/afTransferOperateList.cpp
    template/afLinkedList.cpp
    )

set (initialize_header
    include/AFaccount/AfAccount
    include/AFaccount/AfAccountGroup
    include/AFaccount/AfAccountInfo
    include/AFaccount/AfAccountStorage
    include/AFbaseLib
    include/AFbase/AfAPI
    include/AFbase/AfEnum
    include/AFbase/AfFile
    include/AFbase/AfFunction
    include/AFbase/AfId
    include/AFbase/AfIdHistory
    include/AFbase/AfIdObject
    include/AFbase/AfIdObjectBit
    include/AFbase/AfIdOperate
    include/AFbase/AfPreference
    include/AFbase/AfServerSettings
    include/AFbase/AfSetting
    include/AFbase/AfSystem
    include/AFbase/AfTransferOperateList
    )

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 20)
find_package(Qt6 COMPONENTS Core)

add_library(lib_baseAF ${baseAF_h} ${baseAF_cpp} ${initialize_header})

set_target_properties(lib_baseAF PROPERTIES
    OUTPUT_NAME "baseAF"
    CXX_EXTENSIONS OFF
    )

target_link_libraries(lib_baseAF Qt6::Core)
target_include_directories(lib_baseAF
    PUBLIC  include
    PUBLIC  include/AFbase
    PRIVATE include/lib_baseAF
    )
