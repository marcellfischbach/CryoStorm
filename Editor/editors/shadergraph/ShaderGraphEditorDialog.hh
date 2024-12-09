//
// Created by Marcell on 08.12.2024.
//

#pragma once

#include <QDialog>

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
class csShaderGraph;
class csSGNodeInput;
class csSGNodeOutput;
}

class ShaderGraphNodeItem;
class ShaderGraphEditorDialog : public QDialog
{
Q_OBJECT

public:
  explicit ShaderGraphEditorDialog(QWidget *parent = nullptr);
  ~ShaderGraphEditorDialog() override;

  size_t StartConnectionInput (ShaderGraphNodeItem *item, cs::csSGNodeInput *input, QPointF pos);
  size_t StartConnectionOutput (ShaderGraphNodeItem *item, cs::csSGNodeOutput *output, QPointF pos);
  void UpdateConnection (size_t idx, QPointF pos);
  void CommitConnection (size_t idx);
  void RollbackConnection (size_t idx);

private:
  Ui::ShaderGraphEditorWidget *m_gui;

  QGraphicsScene *m_scene;

  cs::csShaderGraph *m_shaderGraph;

  size_t m_currentDragIdx;
  QGraphicsPathItem* m_currentDragConnection;
};
