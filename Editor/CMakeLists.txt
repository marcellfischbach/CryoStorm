set (CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{QT_HOME}")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL OpenGLWidgets Xml )
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6OpenGL_INCLUDE_DIRS} ${Qt6OpenGLWidgets_INCLUDE_DIRS} ${Qt6Xml_INCLUDE_DIRS})

cmake_policy(SET CMP0100 NEW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(ALL_SOURCES
        main.cc
        mainwindow.cc
        mainwindow.hh
        mainwindow.ui
        Project.cc
        Project.hh

        assetbrowser/assetbrowserdockwidget.cc
        assetbrowser/assetbrowserdockwidget.hh
        assetbrowser/assetbrowserfoldermodel.cc
        assetbrowser/assetbrowserfoldermodel.hh
        assetbrowser/assetbrowsertreemodel.cc
        assetbrowser/assetbrowsertreemodel.hh
        assetbrowser/assetbrowserwidget.cc
        assetbrowser/assetbrowserwidget.hh
        assetbrowser/assetbrowserwidget.ui


        editors/shadergraph/ShaderGraphEditorWidget.cc
        editors/shadergraph/ShaderGraphEditorWidget.hh
        editors/shadergraph/ShaderGraphEditorWidget.ui
        editors/shadergraph/ShaderGraphGraphicsView.cc
        editors/shadergraph/ShaderGraphGraphicsView.hh
        editors/shadergraph/ShaderGraphNodeItem.cc
        editors/shadergraph/ShaderGraphNodeItem.hh
        editors/shadergraph/ShaderGraphNodePalletTreeModel.cc
        editors/shadergraph/ShaderGraphNodePalletTreeModel.hh

        sceneview/SceneViewWidget.cc
        sceneview/SceneViewWidget.hh
        )


add_executable(
        EdLauncher
        ${ALL_SOURCES}
)


target_link_libraries(EdLauncher
        PRIVATE
        csCore
        csOpenGL
        csXml
        Qt6::Core
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        Qt6::Xml
        )

CS_MOC(EdLauncher "")