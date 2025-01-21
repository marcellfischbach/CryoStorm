set (CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{QT_HOME}")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL OpenGLWidgets Xml )
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6OpenGL_INCLUDE_DIRS} ${Qt6OpenGLWidgets_INCLUDE_DIRS} ${Qt6Xml_INCLUDE_DIRS})

cmake_policy(SET CMP0100 NEW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(ALL_SOURCES
        default_registries.cc
        default_registries.hh
        main.cc
        mainwindow.cc
        mainwindow.hh
        mainwindow.ui
        Project.cc
        Project.hh

        assetbrowser/AssetBrowserDockWidget.cc
        assetbrowser/AssetBrowserDockWidget.hh
        assetbrowser/AssetBrowserFolderModel.cc
        assetbrowser/AssetBrowserFolderModel.hh
        assetbrowser/AssetBrowserTreeModel.cc
        assetbrowser/AssetBrowserTreeModel.hh
        assetbrowser/AssetBrowserWidget.cc
        assetbrowser/AssetBrowserWidget.hh
        assetbrowser/AssetBrowserWidget.ui
        assetbrowser/AssetBrowserNewItemDialog.cc
        assetbrowser/AssetBrowserNewItemDialog.hh
        assetbrowser/AssetBrowserNewItemDialog.ui
        assetbrowser/AssetBrowserNewItemRegistry.cc
        assetbrowser/AssetBrowserNewItemRegistry.hh
        assetbrowser/iAssetBrowserNewItem.hh

        editors/EditorRegistry.cc
        editors/EditorRegistry.hh
        editors/iEditorFactory.hh

        editors/shadergraph/ShaderGraphEditorFactory.cc
        editors/shadergraph/ShaderGraphEditorFactory.hh
        editors/shadergraph/ShaderGraphEditorWidget.cc
        editors/shadergraph/ShaderGraphEditorWidget.hh
        editors/shadergraph/ShaderGraphEditorWidget.ui
        editors/shadergraph/ShaderGraphGraphicsView.cc
        editors/shadergraph/ShaderGraphGraphicsView.hh
        editors/shadergraph/ShaderGraphNodeInputWidget.ui
        editors/shadergraph/ShaderGraphNewItem.cc
        editors/shadergraph/ShaderGraphNewItem.hh
        editors/shadergraph/ShaderGraphNodeItem.cc
        editors/shadergraph/ShaderGraphNodeItem.hh
        editors/shadergraph/ShaderGraphNodePalletTreeModel.cc
        editors/shadergraph/ShaderGraphNodePalletTreeModel.hh
        editors/shadergraph/ShaderGraphNodePropertiesWidget.cc
        editors/shadergraph/ShaderGraphNodePropertiesWidget.hh
        editors/shadergraph/ShaderGraphResourceNodeWidget.ui
        editors/shadergraph/ShaderGraphSaver.cc
        editors/shadergraph/ShaderGraphSaver.hh
        editors/shadergraph/ShaderGraphShaderGraphPropertiesWidget.cc
        editors/shadergraph/ShaderGraphShaderGraphPropertiesWidget.hh
        editors/shadergraph/ShaderGraphShaderGraphPropertiesWidget.ui

        sceneview/SceneViewWidget.cc
        sceneview/SceneViewWidget.hh
        )


add_executable(
        EdLauncher
        ${ALL_SOURCES}
)
add_dependencies(EdLauncher csAssimpLoader)
add_dependencies(EdLauncher csBullet)
add_dependencies(EdLauncher csImgLoader)
add_dependencies(EdLauncher csOpenGL)


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


add_dependencies(EdLauncher csMOC)
CS_MOC(EdLauncher "")