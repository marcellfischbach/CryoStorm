//
// Created by Marcell on 08.12.2024.
//

#pragma once

#include <csCore/resource/csAssetLocator.hh>
#include <csCore/csRef.hh>
#include <csEditor/editors/EditorWidget.hh>

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
class ShaderGraphEditorWidget : public EditorWidget
{
Q_OBJECT

public:
  explicit ShaderGraphEditorWidget(cs::csShaderGraph *shaderGraph,
                                   const cs::csAssetLocator &locator,
                                   QWidget *parent = nullptr);
  ~ShaderGraphEditorWidget() override;


private:
  void onBtnSaveClicked(bool);

private slots:
  void onPreviewInitialized(cs::csWorld *world);
  void onPreviewMousePressed(QMouseEvent *event);
  void onPreviewMouseReleased(QMouseEvent *event);
  void onPreviewMouseMoved(QMouseEvent *event);

  void on_btnCompile_clicked();
  void onNodeSelectionChanged();
  void on_graph_ConnectionsChanged();

  void onResourceNameChanged();


private:
  bool CompileMaterial();

  bool   m_leftButtonArmed  = false;
  bool   m_rightButtonArmed = false;
  double m_cameraRotationH  = 0.0;
  double m_cameraRotationV  = 0.0;
  double m_lightRotationH   = 0.0;
  double m_lightRotationV   = 0.0;
  QPoint m_buttonPos;

  double RotationHFrom(double baseRotation, const QPoint &point, bool invert);
  double RotationVFrom(double baseRotation, const QPoint &point, bool invert4);
  void UpdateCamera(double rotationH, double rotationV);
  void UpdateLight(double rotationH, double rotationV);


  Ui::ShaderGraphEditorWidget *m_gui;


  cs::csRef<cs::csWorld>       m_world        = nullptr;
  cs::csRef<cs::csEntity>      m_cameraEntity = nullptr;
  cs::csRef<cs::csCameraState> m_camera       = nullptr;

  cs::csRef<cs::csEntity>     m_lightEntity = nullptr;
  cs::csRef<cs::csLightState> m_light       = nullptr;

  cs::csAssetRef<cs::csShaderGraph> m_shaderGraph  = nullptr;
  cs::csAssetLocator                m_locator;
  ShaderGraphNodePalletTreeModel    *m_palletModel = nullptr;

  cs::csRef<cs::csStaticMeshState> m_cube       = nullptr;
  cs::csRef<cs::csEntity>          m_cubeEntity = nullptr;


};
