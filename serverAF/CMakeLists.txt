cmake_minimum_required(VERSION 3.14)
project(server_accountAF)

set (server_accountAF_src
    dataServer.cpp
    dataServer.h
    accountServer.cpp
    accountServer.h
    main.cpp
)

find_package(Qt5Core)
find_package(Qt5Network)

add_executable(server_accountAF ${server_accountAF_src})
target_link_libraries(server_accountAF
    lib_baseAF qt_http_server
    Qt5::Core Qt5::Network)

target_include_directories(server_accountAF
    PUBLIC $<TARGET_PROPERTY:lib_baseAF,INTERFACE_INCLUDE_DIRECTORIES>
    PUBLIC $<TARGET_PROPERTY:qt_http_server,INTERFACE_INCLUDE_DIRECTORIES>
)
