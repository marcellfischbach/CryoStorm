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

        editors/EditorDialog.cc
        editors/EditorDialog.hh
        editors/EditorRegistry.cc
        editors/EditorRegistry.hh
        editors/EditorWidget.cc
        editors/EditorWidget.hh
        editors/iEditorFactory.hh

        editors/image/ImageEditorFactory.cc
        editors/image/ImageEditorFactory.hh
        editors/image/ImageEditorWidget.cc
        editors/image/ImageEditorWidget.hh
        editors/image/ImageWidget.cc
        editors/image/ImageWidget.hh



        editors/shadergraph/ShaderGraphEditorFactory.cc
        editors/shadergraph/ShaderGraphEditorFactory.hh
        editors/shadergraph/ShaderGraphEditorNodePropertiesWidget.cc
        editors/shadergraph/ShaderGraphEditorNodePropertiesWidget.hh
        editors/shadergraph/ShaderGraphEditorWidget.cc
        editors/shadergraph/ShaderGraphEditorWidget.hh
        editors/shadergraph/ShaderGraphEditorWidget.ui
        editors/shadergraph/ShaderGraphGraphicsView.cc
        editors/shadergraph/ShaderGraphGraphicsView.hh
        editors/shadergraph/ShaderGraphNewItem.cc
        editors/shadergraph/ShaderGraphNewItem.hh
        editors/shadergraph/ShaderGraphNodeItem.cc
        editors/shadergraph/ShaderGraphNodeItem.hh
        editors/shadergraph/ShaderGraphNodePalletTreeModel.cc
        editors/shadergraph/ShaderGraphNodePalletTreeModel.hh
        editors/shadergraph/ShaderGraphSaver.cc
        editors/shadergraph/ShaderGraphSaver.hh

        sceneview/SceneViewWidget.cc
        sceneview/SceneViewWidget.hh
        )


add_executable(
        csEditor
        ${ALL_SOURCES}
)
add_dependencies(csEditor csAssimpLoader)
add_dependencies(csEditor csBullet)
add_dependencies(csEditor csImgLoader)
add_dependencies(csEditor csOpenGL)


target_link_libraries(csEditor
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


add_dependencies(csEditor csMOC)
CS_MOC(csEditor "")