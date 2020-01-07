cmake_minimum_required(VERSION 3.14)
project(lib_accountAF VERSION 0.0.1)

set (accountAF_h
    include/lib_accountAF/account.h
    include/lib_accountAF/accountAPI.h
    include/lib_accountAF/accountGroup.h
    include/lib_accountAF/accountInfo.h
    include/lib_accountAF/accountStorage.h
)

set (accountAF_cpp
    src/account.cpp
    src/accountAPI.cpp
    src/accountGroup.cpp
    src/accountInfo.cpp
    src/accountStorage.cpp
)

find_package(Qt5Core)

add_library(lib_accountAF SHARED ${accountAF_h} ${accountAF_cpp})

set_target_properties(lib_accountAF PROPERTIES OUTPUT_NAME "accountAF")
target_link_libraries(lib_accountAF lib_baseAF lib_currencyAF Qt5::Core)
target_include_directories(lib_accountAF
    PUBLIC  include
    PRIVATE include/lib_accountAF
    PUBLIC $<TARGET_PROPERTY:lib_baseAF,INTERFACE_INCLUDE_DIRECTORIES>
    PUBLIC $<TARGET_PROPERTY:lib_currencyAF,INTERFACE_INCLUDE_DIRECTORIES>
)
