//
// Created by Marcell on 08.12.2024.
//

#pragma once

#include <QObject>
#include <QRectF>
#include <QGraphicsRectItem>

class QGraphicsScene;
class QGraphicsEllipseItem;
class QGraphicsRectItem;
class QGraphicsTextItem;

namespace cs
{
class csSGNodeInput;
class csSGNodeOutput;
class csSGNode;
}

class ShaderGraphEditorDialog;

class ShaderGraphNodeItem : public QGraphicsRectItem
{
public:

  ShaderGraphNodeItem(cs::csSGNode *node, ShaderGraphEditorDialog *dlg);
  ~ShaderGraphNodeItem() override;

  enum
  {
    CUSTOM_DATA_IS_NODE_ITEM = 1
  };

  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  void GenerateSurroundingRect();
  void GenerateInputsAndOutputs();
  void CalculateSizes();
  void UpdateSizeAndPositions();
  QRectF CalculateTotalSize();


  ShaderGraphEditorDialog *m_dlg;
  cs::csSGNode            *m_node;
  QGraphicsRectItem       *m_titleRect;
  QGraphicsTextItem       *m_title;

  bool    m_dragNode;
  QPointF m_dragNodeMoveStartPointer;
  QPointF m_dragNodeMoveStartPosition;

  bool m_dragHandle;
  QPointF m_dragHandleMoveStartPointer;
  size_t m_dragHandleIdx;
  size_t m_dragHandleConnectionIdx;



  struct InputOutputHandle
  {
    cs::csSGNodeInput    *Input  = nullptr;
    cs::csSGNodeOutput   *Output = nullptr;
    QGraphicsEllipseItem *Handle = nullptr;
    QGraphicsTextItem    *Text   = nullptr;
    QRectF               Size;
  };


  std::vector<InputOutputHandle> m_handles;
};
