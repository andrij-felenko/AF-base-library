cmake_minimum_required(VERSION 3.19)
project(server_accountAF VERSION 0.0.1)

set (server_accountAF_src
    server.cpp
    server.h
    main.cpp
    )

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 20)
find_package(Qt6 COMPONENTS Core Network)

add_executable(server_accountAF ${server_accountAF_src})
target_link_libraries(server_accountAF
    lib_baseAF qt_http_server
    Qt6::Core Qt6::Network
    )

target_include_directories(server_accountAF
    PUBLIC $<TARGET_PROPERTY:lib_baseAF,INTERFACE_INCLUDE_DIRECTORIES>
    PUBLIC $<TARGET_PROPERTY:qt_http_server,INTERFACE_INCLUDE_DIRECTORIES>
    )
