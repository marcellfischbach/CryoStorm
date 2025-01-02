//
// Created by Marcell on 08.12.2024.
//

#pragma once

#include <csCore/resource/csResourceLocator.hh>

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
  explicit ShaderGraphEditorWidget(cs::csShaderGraph* shaderGraph, const cs::csResourceLocator &locator, QWidget *parent = nullptr);
  ~ShaderGraphEditorWidget() override;


private:
  void onBtnSaveClicked(bool);

private slots:
  void on_preview_initialize(cs::csWorld* world);
  void on_btnCompile_clicked();
  void on_graph_SelectionChanged ();
  void on_graph_ConnectionsChanged ();
  void CompileMaterial();

private:



  Ui::ShaderGraphEditorWidget *m_gui;


  cs::csWorld* m_world = nullptr;
  cs::csEntity* m_cameraEntity = nullptr;
  cs::csCameraState *m_camera = nullptr;

  cs::csEntity* m_lightEntity = nullptr;
  cs::csLightState *m_light = nullptr;

  cs::csShaderGraph *m_shaderGraph = nullptr;
  cs::csResourceLocator m_locator;
  ShaderGraphNodePalletTreeModel *m_palletModel = nullptr;

  cs::csStaticMeshState* m_cube = nullptr;
  cs::csEntity* m_cubeEntity = nullptr;

  cs::csMaterial* m_material = nullptr;

};
