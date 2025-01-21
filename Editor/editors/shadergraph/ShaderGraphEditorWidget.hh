//
// Created by Marcell on 08.12.2024.
//

#pragma once

#include <csCore/resource/csAssetLocator.hh>
#include <csCore/csRef.hh>

#include <QDialog>
#include <vector>
#include <set>


QT_BEGIN_NAMESPACE
namespace Ui
{
class ShaderGraphEditorWidget;
}
QT_END_NAMESPACE

class QGraphicsScene;
class QGraphicsPathItem;
namespace cs
{
class csCameraState;
class csClass;
class csEntity;
class csMaterial;
class csLightState;
class csStaticMeshState;
class csShaderGraph;
class csSGNode;
class csSGNodeInput;
class csSGNodeIO;
class csSGNodeOutput;
class csWorld;
}

class ShaderGraphNodeItem;
class ShaderGraphNodePalletTreeModel;
class ShaderGraphEditorWidget : public QDialog
{
Q_OBJECT

public:
  explicit ShaderGraphEditorWidget(cs::csShaderGraph* shaderGraph, const cs::csAssetLocator &locator, QWidget *parent = nullptr);
  ~ShaderGraphEditorWidget() override;


private:
  void onBtnSaveClicked(bool);

private slots:
  void on_preview_initialize(cs::csWorld* world);
  void on_btnCompile_clicked();
  void on_graph_SelectionChanged ();
  void on_graph_ConnectionsChanged ();

private:
  bool CompileMaterial();



  Ui::ShaderGraphEditorWidget *m_gui;


  cs::csRef<cs::csWorld> m_world = nullptr;
  cs::csRef<cs::csEntity> m_cameraEntity = nullptr;
  cs::csRef<cs::csCameraState> m_camera = nullptr;

  cs::csRef<cs::csEntity> m_lightEntity = nullptr;
  cs::csRef<cs::csLightState> m_light = nullptr;

  cs::csAssetRef<cs::csShaderGraph> m_shaderGraph = nullptr;
  cs::csAssetLocator             m_locator;
  ShaderGraphNodePalletTreeModel *m_palletModel = nullptr;

  cs::csRef<cs::csStaticMeshState> m_cube = nullptr;
  cs::csRef<cs::csEntity> m_cubeEntity = nullptr;


};
