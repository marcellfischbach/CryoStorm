//
// Created by Marcell on 08.12.2024.
//

#pragma once

#include <QDialog>
#include <vector>

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
class csClass;
class csShaderGraph;
class csSGNode;
class csSGNodeInput;
class csSGNodeIO;
class csSGNodeOutput;
}

class ShaderGraphNodeItem;
class ShaderGraphNodePalletTreeModel;
class ShaderGraphEditorWidget : public QDialog
{
Q_OBJECT

public:
  explicit ShaderGraphEditorWidget(QWidget *parent = nullptr);
  ~ShaderGraphEditorWidget() override;

  size_t StartConnection(ShaderGraphNodeItem *item, cs::csSGNodeIO *io, QPointF pos);
  void UpdateConnection(size_t idx, QPointF scenePos);
  void CommitConnection(size_t idx, QPointF scenePos);
  void RollbackConnection(size_t idx);

  void UpdateNodeLocation(ShaderGraphNodeItem *node);

  void RemoveWire(cs::csSGNodeInput *input);
protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

private slots:
  void on_graph_dropNode (const cs::csClass* cls, const QPointF &pos);

private:
  void InsertNode (cs::csSGNode *node, const QPointF &scenePos);

  struct Wire
  {
    QGraphicsPathItem *Path;
    ShaderGraphNodeItem *Source;
    cs::csSGNodeOutput *SourceIO;
    ShaderGraphNodeItem *Destination;
    cs::csSGNodeInput *DestinationIO;
  };

  void UpdateWires();
  void UpdateWire(Wire &wire);
  void WireIOs(ShaderGraphNodeItem *node0, cs::csSGNodeIO *io0, ShaderGraphNodeItem *node1, cs::csSGNodeIO *io1);


  ShaderGraphNodeItem *At(const QPointF &scenePoint);
  Ui::ShaderGraphEditorWidget *m_gui;

  QGraphicsScene *m_scene;

  cs::csShaderGraph *m_shaderGraph;
  ShaderGraphNodePalletTreeModel *m_palletModel;

  size_t m_currentDragIdx;
  QGraphicsPathItem *m_currentDragConnection;

  std::vector<ShaderGraphNodeItem *> m_nodes;

  ShaderGraphNodeItem *m_wireStartNode;
  cs::csSGNodeIO *m_wireStartIO;


  std::vector<Wire> m_wires;
};
