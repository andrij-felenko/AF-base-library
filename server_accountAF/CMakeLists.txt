#cmake_minimum_required(VERSION 3.14)
#project(lib_baseAF VERSION 0.0.1)

#set (baseAF_h
#    include/account.h
#    include/accountLocalStorage.h
#    include/accountServer.h
#    include/accountStorage.h
#    include/applicationAF.h
#    include/crypt.h
#    include/dir.h
#    include/enumAF.h
#    include/file.h
#    include/function.h
#    include/id.h
#    include/libHandler.h
#    include/libSingle.h
#    include/origin.h
#    include/plugin.h
#    include/pluginHandler.h
#    include/pluginMetadata.h
#    include/pluginSingle.h
#    include/setting.h
#    include/storage.h
#    include/system.h
#    include/version.h
#)

#set (baseAF_cpp
#    account/account.cpp
#    account/accountLocalStorage.cpp
#    account/accountServer.cpp
#    account/accountStorage.cpp
#    account/id.cpp
#    loader/libHandler.cpp
#    loader/libSingle.cpp
#    loader/plugin.cpp
#    loader/pluginHandler.cpp
#    loader/pluginMetadata.cpp
#    loader/pluginSingle.cpp
#    loader/version.cpp
#    tool/crypt.cpp
#    tool/dir.cpp
#    tool/enumAF.cpp
#    tool/file.cpp
#    tool/function.cpp
#    tool/origin.cpp
#    tool/setting.cpp
#    tool/storage.cpp
#    tool/system.cpp
#)

#set (CMAKE_AUTORCC)
#find_package(Qt5Core)

#add_library(lib_baseAF SHARED ${baseAF_h} ${baseAF_cpp})

#set_target_properties(lib_baseAF PROPERTIES OUTPUT_NAME "baseAF")
#target_link_libraries(lib_baseAF Qt5::Core)
#target_include_directories(lib_baseAF PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
## TODO cmake target properties, how it work?
##set_target_properties(lib_baseAF PROPERTIES PUBLIC_HEADER ${baseAF_h})
