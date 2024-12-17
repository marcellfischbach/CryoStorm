//
// Created by Marcell on 12.12.2024.
//

#pragma once

#include <QGraphicsView>
#include <map>
#include <vector>
#include <set>


namespace cs
{
class csClass;
class csSGNode;
class csSGNodeIO;
class csSGNodeInput;
class csSGNodeOutput;
class csShaderGraph;
}

class ShaderGraphNodeItem;
class ShaderGraphGraphicsView : public QGraphicsView
{
Q_OBJECT
public:
  ShaderGraphGraphicsView(QWidget *parent = nullptr);
  void SetShaderGraph (cs::csShaderGraph* shaderGraph);

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

  void keyReleaseEvent (QKeyEvent *event) override;

  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;


private:



  ShaderGraphNodeItem *FindNode(const cs::csSGNode *node);
  ShaderGraphNodeItem *FindNode(const QPointF &scenePoint);



  void InsertNode(const cs::csClass *cls, const QPointF &position);
  void InsertNode(cs::csSGNode *node, const QPointF &scenePos);

  void HandleSelection(const QPointF &scenePos, QMouseEvent *event);
  void LastMinuteSelection(const QPointF &scenePos, QMouseEvent *event);
  void ClearSelection();
  void AddSelection(ShaderGraphNodeItem *node);
  void RemoveSelection(ShaderGraphNodeItem *node);

  bool IsSelected(ShaderGraphNodeItem *node) const;
  void BeginDragSelectedNodes(const QPointF &scenePos);
  void UpdateDragSelectedNodes(const QPointF &scenePos);
  void CommitDragSelectedNodes(const QPointF &scenePos);
  void RollbackDragSelectedNodes();

  void BeginDragIO(ShaderGraphNodeItem *node, cs::csSGNodeIO *io, const QPointF &scenePos);
  void UpdateDragIO(const QPointF &scenePos);
  void CommitDragIO(const QPointF &scenePos);
  void RollbackDragIO();

  bool CanConnect(ShaderGraphNodeItem *srcNode, cs::csSGNodeIO *srcIO,
                  ShaderGraphNodeItem *dstNode, cs::csSGNodeIO *dstIO) const;

  void ClearDragIO();
  void ConnectDragIO();

  void CreateWire(ShaderGraphNodeItem *outputNode,
                  cs::csSGNodeOutput *output,
                  ShaderGraphNodeItem *inputNode,
                  cs::csSGNodeInput *input);
  void RemoveWireToNode(ShaderGraphNodeItem *node);
  void RemoveWireToInput(ShaderGraphNodeItem *inputNode,
                         cs::csSGNodeInput *input);
  struct Wire;
  void UpdateWires();
  void UpdateWiresOfSelectedNodes();
  void UpdateWire(Wire &wire);
  void UpdateWireHandles (Wire &wire);
  void RegenerateWires ();
  void RegenerateInputWires(cs::csSGNode* node);

  void ClearAll();
  void RemoveWire(Wire &wire);
  void DeleteNode (ShaderGraphNodeItem* node);


  bool m_leftButtonDown = false;

  struct
  {
    bool primed;
    QPoint point;
  } m_nodeSelection;

  struct
  {
    QPointF startPosClick;
    std::map<ShaderGraphNodeItem *, QPointF> startPosNodes;
    bool IsDragging() const
    {
      return !startPosNodes.empty();
    }
  } m_nodeDrag;


  struct
  {
    ShaderGraphNodeItem *srcNode = nullptr;
    cs::csSGNodeIO *srcIO = nullptr;
    ShaderGraphNodeItem *dstNode = nullptr;
    cs::csSGNodeIO *dstIO = nullptr;

    QPointF startPos;
    QGraphicsPathItem *graphItem = nullptr;

    bool IsDragging() const
    {
      return srcNode;
    }

  } m_ioDrag;

  bool IsDragIOComplete () const;
  bool IsDragIOClearSource () const;


  const cs::csClass *ClsFrom(QDropEvent *event) const;
  struct Wire
  {
    QGraphicsPathItem *Path;
    ShaderGraphNodeItem *Source;
    cs::csSGNodeOutput *SourceIO;
    ShaderGraphNodeItem *Destination;
    cs::csSGNodeInput *DestinationIO;
    bool operator==(const Wire &wire) const;
    bool IsValid () const;
  };

  cs::csShaderGraph *m_shaderGraph;

  std::vector<ShaderGraphNodeItem *> m_nodes;
  std::set<ShaderGraphNodeItem *> m_selectedNodes;
  std::vector<Wire> m_wires;

  QGraphicsScene *m_scene;

  std::map<const cs::csClass*, size_t> m_defaultIdx;

};
