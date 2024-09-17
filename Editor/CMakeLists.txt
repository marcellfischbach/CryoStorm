message ("CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")
set (CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};D:/DEV/Qt/6.8.0/msvc2019_64")

find_package(Qt6 COMPONENTS Core Widgets OpenGL Xml REQUIRED)
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6OpenGL_INCLUDE_DIRS} ${Qt6Xml_INCLUDE_DIRS})

cmake_policy(SET CMP0100 NEW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(ALL_SOURCES
        main.cc
        mainwindow.cc
        mainwindow.hh
        mainwindow.ui

        assetbrowser/assetbrowserdockwidget.cc
        assetbrowser/assetbrowserdockwidget.hh
        assetbrowser/assetbrowserfoldermodel.cc
        assetbrowser/assetbrowserfoldermodel.hh
        assetbrowser/assetbrowsertreemodel.cc
        assetbrowser/assetbrowsertreemodel.hh
        assetbrowser/assetbrowserwidget.cc
        assetbrowser/assetbrowserwidget.hh
        assetbrowser/assetbrowserwidget.ui

        )


add_executable(
        EdLauncher
        ${ALL_SOURCES}
)


target_link_libraries(EdLauncher
        Qt6::Core
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::Xml
        ceCore
        ceOpenGL
        )

CE_MOC(EdLauncher)