//
// Created by Marcell on 08.12.2024.
//

#pragma once

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
  explicit ShaderGraphEditorWidget(QWidget *parent = nullptr);
  ~ShaderGraphEditorWidget() override;

private slots:
  void on_graph_initialize(cs::csWorld* world);

private:


  Ui::ShaderGraphEditorWidget *m_gui;


  cs::csWorld* m_world = nullptr;
  cs::csEntity* m_cameraEntity = nullptr;
  cs::csCameraState *m_camera = nullptr;

  cs::csShaderGraph *m_shaderGraph;
  ShaderGraphNodePalletTreeModel *m_palletModel;


};
