cmake_minimum_required(VERSION 3.16)
project(DiagramEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

find_package(Qt5 COMPONENTS 
    Core 
    Gui 
    Widgets 
    Svg 
    Network
    REQUIRED
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
    canvas.h
    canvasdiagram.h
    canvasdrawstragety.h
    diagram.h
    diagramdrawparams.h
    drawer.h
    drawparamscreator.h
    drawresult.h
    myconfig.h
    mymainwindow.h
    namespace.h
    propertydata.h
    propertydatabuilder.h
    propertydelegate.h
    propertydelegateparams.h
    propertynamevec.h
    propertyset.h
    propertywidget.h
    canvas.cpp 
    canvasdiagram.cpp
    canvasdrawstragety.cpp
    diagram.cpp
    diagramdrawparams.cpp
    drawer.cpp
    drawparamscreator.cpp
    drawresult.cpp
    main.cpp
    myconfig.cpp
    mymainwindow.cpp
    namespace.cpp
    propertydata.cpp
    propertydatabuilder.cpp
    propertydelegate.cpp
    propertydelegateparams.cpp
    propertynamevec.cpp
    propertyset.cpp
    propertywidget.cpp
    devconfig.qrc
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
    Qt5::Svg
)
