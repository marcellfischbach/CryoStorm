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
class csSGNodeIO;
class csSGNodeOutput;
class csSGNode;
}

class ShaderGraphEditorWidget;

class ShaderGraphNodeItem : public QGraphicsRectItem
{
public:

  ShaderGraphNodeItem(cs::csSGNode *node, ShaderGraphEditorWidget *editorWidget);
  ~ShaderGraphNodeItem() override;

  void SetSelected (bool  selected);
  bool IsHeader (const QPointF &scenePos);
  cs::csSGNodeIO *IoAt(const QPointF &scenePos);
  const cs::csSGNodeIO *IoAt(const QPointF &scenePos) const;


  void UpdateHandles ();


  cs::csSGNode *GetNode();

  QRectF IoSceneRectAt(const QPointF &scenePos) const;

  QPointF ScenePosOf(const cs::csSGNodeIO *io) const;

  enum
  {
    CUSTOM_DATA_IS_NODE_ITEM = 1
  };

  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  struct InputOutputHandle;
  const InputOutputHandle *IoHandleAt(const QPointF &scenePos) const;
  void GenerateSurroundingRect();
  void GenerateInputsAndOutputs();
  void CalculateSizes();
  void UpdateSizeAndPositions();
  QRectF CalculateTotalSize();

  struct InputOutputHandle;
  void UpdateHandle (InputOutputHandle &handle);


  ShaderGraphEditorWidget *m_edito2rWidget;
  cs::csSGNode *m_node;
  QGraphicsRectItem *m_titleRect;
  QGraphicsTextItem *m_title;

  bool m_dragNode;
  QPointF m_dragNodeMoveStartPointer;
  QPointF m_dragNodeMoveStartPosition;

  struct InputOutputHandle;
  const InputOutputHandle *m_dragHandle;
  QPointF m_dragHandleMoveStartPointer;
  size_t m_dragHandleConnectionIdx;


  struct InputOutputHandle
  {
    cs::csSGNodeIO *IO = nullptr;
    QGraphicsEllipseItem *Handle = nullptr;
    QGraphicsEllipseItem *HandleDot = nullptr;
    QGraphicsTextItem *Text = nullptr;
    QRectF Size;
  };

  std::vector<InputOutputHandle> m_handles;
};
