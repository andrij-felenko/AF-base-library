cmake_minimum_required(VERSION 3.14)
project(lib_quickAF VERSION 0.0.1)

set (quickAF_h
    include/lib_quickAF/guiApplicationAF.h
    include/lib_quickAF/pluginGui.h
    include/lib_quickAF/pluginGuiHandler.h
    include/lib_quickAF/quickAFlib.h
    )

set (quickAF_cpp
    src/pluginGui.cpp
    src/pluginGuiHandler.cpp
    src/quickAFlib.cpp
    )

set (CMAKE_AUTORCC)
find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Quick)

add_library(lib_quickAF SHARED ${quickAF_h} ${quickAF_cpp})

set_target_properties(lib_quickAF PROPERTIES OUTPUT_NAME "quickAF")
target_link_libraries(lib_quickAF lib_baseAF Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml)
target_include_directories(lib_quickAF
    PUBLIC include
    PRIVATE include/lib_quickAF
    PUBLIC $<TARGET_PROPERTY:lib_baseAF,INTERFACE_INCLUDE_DIRECTORIES>
)
