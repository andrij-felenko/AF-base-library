cmake_minimum_required(VERSION 3.19)
project(lib_quickAF VERSION 0.0.1)

set (quickAF_h
    include/lib_quickAF/guiApplication.h
    include/lib_quickAF/guiPlugin.h
    include/lib_quickAF/guiSize.h
    include/lib_quickAF/menuModel.h
    )

set (quickAF_cpp
    src/guiApplication.cpp
    src/guiPlugin.cpp
    src/guiSize.cpp
    src/menuModel.cpp
    )

set (initialize_header
    include/AFquick/AfGuiApplication
    include/AFquick/AfGuiPlugin
    include/AFquick/AfGuiSize
    include/AFquick/AfMenuModel
    include/AFquickLib
    )

set (force_rc_material_design -Wl,--whole-archive rc_material_design -Wl,--no-whole-archive)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_CXX_STANDARD 20)
find_package(Qt6 COMPONENTS Core Qml Quick)

add_library(lib_quickAF
    ${quickAF_h} ${quickAF_cpp}
    ${initialize_header}
    rc/af_quick_rc.qrc
    )

set_target_properties(lib_quickAF PROPERTIES OUTPUT_NAME "quickAF")
target_link_libraries(lib_quickAF lib_baseAF
    ${force_rc_material_design}
    Qt6::Core Qt6::Gui Qt6::Quick Qt6::Qml
    )

target_include_directories(lib_quickAF
    PUBLIC include
    PUBLIC include/AFquick
    PRIVATE include/lib_quickAF
    PUBLIC $<TARGET_PROPERTY:lib_baseAF,INTERFACE_INCLUDE_DIRECTORIES>
    )
