//
// Created by Marcell on 08.12.2024.
//

#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <editors/shadergraph/ShaderGraphEditorWidget.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <csCore/graphics/shadergraph/csSGNodes.hh>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

using namespace cs;

static const double HANDLE_SIZE = 16.0;
static const double HANDLE_DOT_SIZE = 8.0;
static const double HANDLE_DOT_OFFSET = (HANDLE_SIZE - HANDLE_DOT_SIZE) / 2.0;
static const double HANDLE_MARGIN = 4.0;
static const double HANDLE_TEXT_GAP = 2.0;

static const double TITLE_MARGIN = 1.0;
static const double TITLE_BODY_GAP = 4.0;
static const double INPUT_OUTPUT_GAP = 16.0;

ShaderGraphNodeItem::ShaderGraphNodeItem(csSGNode *node, ShaderGraphEditorWidget *editorWidget)
    : QGraphicsRectItem()
    , m_node(nullptr)
//    , m_editorWidget(editorWidget)
    , m_dragNode(false)
    , m_dragHandle(nullptr)
{
  m_node = node;
  setData(CUSTOM_DATA_IS_NODE_ITEM, true);

  GenerateSurroundingRect();
  GenerateInputsAndOutputs();
  CalculateSizes();

  UpdateSizeAndPositions();
  UpdateHandlesAndResource();
}


ShaderGraphNodeItem::~ShaderGraphNodeItem()
{
}


cs::csSGNode *ShaderGraphNodeItem::GetNode()
{
  return m_node;
}

void ShaderGraphNodeItem::SetSelected(bool selected)
{
  setPen(selected
         ? QPen(QColor(255, 255, 255), 2)
         : Qt::NoPen
  );
}

bool ShaderGraphNodeItem::IsHeader(const QPointF &scenePos)
{
  QPointF localPos = mapFromScene(scenePos);
  return m_titleRect->contains(localPos);
}

cs::csSGNodeIO *ShaderGraphNodeItem::IoAt(const QPointF &scenePos)
{
  return const_cast<cs::csSGNodeIO *>(static_cast<const ShaderGraphNodeItem *>(this)->IoAt(scenePos));
}

const cs::csSGNodeIO *ShaderGraphNodeItem::IoAt(const QPointF &scenePos) const
{
  const InputOutputHandle *handle = IoHandleAt(scenePos);
  return handle ? handle->IO : nullptr;
}

QRectF ShaderGraphNodeItem::IoSceneRectAt(const QPointF &scenePos) const
{
  const InputOutputHandle *handle = IoHandleAt(scenePos);
  if (!handle)
  {

    return QRectF();
  }

  const QRectF &rect = handle->Handle->rect();
  return mapRectToScene(rect);
}

const ShaderGraphNodeItem::InputOutputHandle *ShaderGraphNodeItem::IoHandleAt(const QPointF &scenePos) const
{
  QPointF localPos = mapFromScene(scenePos);
  for (const auto &handle: m_handles)
  {
    if (handle.Handle->contains(localPos))
    {
      return &handle;
    }
  }
  return nullptr;
}

void ShaderGraphNodeItem::SyncPosition()
{
  QPointF pos = scenePos();
  m_node->SetPosition(csVector2f(pos.x(), pos.y()));
}

QPointF ShaderGraphNodeItem::ScenePosOf(const cs::csSGNodeIO *io) const
{
  for (const auto &handle: m_handles)
  {
    if (handle.IO == io)
    {
      return mapToScene(handle.Handle->rect().center());
    }
  }
  return QPointF();
}

void ShaderGraphNodeItem::GenerateSurroundingRect()
{

  setBrush(QBrush(QColor(32, 32, 32, 255)));
  setPen(QPen(QColor(32, 32, 32, 255)));
  
  QColor rectColor = QColor(0, 0, 192);
  
  m_titleRect = new QGraphicsRectItem(0, 0, 198, 50, this);

  m_title = new QGraphicsTextItem(m_node->GetName().c_str(), m_titleRect);
  m_title->setDefaultTextColor(QColor(255, 255, 255, 255));
  m_title->setPos(TITLE_MARGIN, TITLE_MARGIN);

  if (m_node->IsInstanceOf<csSGResourceNode>())
  {
    csSGResourceNode *resource = m_node->Query<csSGResourceNode>();
    m_resourceName = new QGraphicsTextItem(resource->GetResourceName().c_str(), m_titleRect);
    m_resourceName->setDefaultTextColor(QColor(255, 255, 255, 255));
    m_resourceName->setPos(TITLE_MARGIN, TITLE_MARGIN);

    rectColor = QColor(192, 0, 0, 255);
  }
  else
  {
    bool allInputAreConst = true;
    for (int i = 0; i < m_node->GetNumberOfInputs(); ++i)
    {
      csSGNodeInput *input = m_node->GetInput(i);
      if (!input->IsConst())
      {
        allInputAreConst = false;
      }
    }
    if (allInputAreConst)
    {
      rectColor = QColor(192, 192, 0);
    }
  }
  


  m_titleRect->setBrush(QBrush(rectColor));
  m_titleRect->setPen(QPen(rectColor));
}

void ShaderGraphNodeItem::GenerateInputsAndOutputs()
{

  QColor whiteColor(255, 255, 255);
  for (int i = 0; i < m_node->GetNumberOfInputs(); i++)
  {
    csSGNodeInput *input = m_node->GetInput(i);
    if (input->IsConst())
    {
      continue;
    }
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0, 0, HANDLE_SIZE, HANDLE_SIZE, this);
    QGraphicsEllipseItem *ellipseItemDot = new QGraphicsEllipseItem(HANDLE_DOT_OFFSET, HANDLE_DOT_OFFSET,
                                                                    HANDLE_DOT_SIZE, HANDLE_DOT_SIZE,
                                                                    this);
    QGraphicsTextItem *textItem = new QGraphicsTextItem(input->GetName().c_str(), this);
    ellipseItem->setPen(QPen(whiteColor));
    ellipseItemDot->setPen(Qt::NoPen);
    ellipseItemDot->setBrush(Qt::NoBrush);
    textItem->setDefaultTextColor(whiteColor);
    m_handles.emplace_back(input, ellipseItem, ellipseItemDot, textItem, QRectF());
  }
  for (int i = 0; i < m_node->GetNumberOfOutputs(); i++)
  {
    csSGNodeOutput *output = m_node->GetOutput(i);
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0, 0, HANDLE_SIZE, HANDLE_SIZE, this);
    QGraphicsEllipseItem *ellipseItemDot = new QGraphicsEllipseItem(HANDLE_DOT_OFFSET, HANDLE_DOT_OFFSET,
                                                                    HANDLE_DOT_SIZE, HANDLE_DOT_SIZE,
                                                                    this);
    QGraphicsTextItem *textItem = new QGraphicsTextItem(output->GetName().c_str(), this);
    ellipseItem->setPen(QPen(whiteColor));
    ellipseItemDot->setPen(Qt::NoPen);
    ellipseItemDot->setBrush(Qt::NoBrush);
    textItem->setDefaultTextColor(whiteColor);
    m_handles.emplace_back(output, ellipseItem, ellipseItemDot, textItem, QRectF());
  }
}

void ShaderGraphNodeItem::CalculateSizes()
{
  for (auto &handle: m_handles)
  {
    double handleWidth = HANDLE_SIZE + HANDLE_MARGIN + HANDLE_TEXT_GAP;
    double handleHeight = HANDLE_SIZE + HANDLE_MARGIN; // only one margin because the margin on the other side comes
    // from the handle item blow or above

    const QRectF &textSize = handle.Text->boundingRect();
    handleWidth += textSize.width();
    handleHeight = std::max(handleHeight, textSize.height());

    handle.Size = QRectF(0.0, 0.0, handleWidth, handleHeight);
  }
}


QRectF ShaderGraphNodeItem::CalculateTotalSize()
{
  QRectF titleSize = m_title->boundingRect();

  double inputWidth = 0.0;
  double inputHeight = 0.0;
  double outputWidth = 0.0;
  double outputHeight = 0.0;
  for (const auto &item: m_handles)
  {
    if (item.IO->IsInstanceOf<csSGNodeInput>())
    {
      inputWidth = std::max(inputWidth, item.Size.width());
      inputHeight += item.Size.height();
    }
    if (item.IO->IsInstanceOf<csSGNodeOutput>())
    {
      outputWidth = std::max(outputWidth, item.Size.width());
      outputHeight += item.Size.height();
    }
  }

  double totalWidth = inputWidth + outputWidth + INPUT_OUTPUT_GAP;
  double totalHeight = std::max(inputHeight, outputHeight);

  totalWidth = std::max(totalWidth, titleSize.width() + TITLE_MARGIN * 2);
  totalHeight += titleSize.height() + TITLE_BODY_GAP + 2;
  if (m_resourceName)
  {
    const QRectF &resourceSize = m_resourceName->boundingRect();

    totalWidth = std::max(totalWidth, resourceSize.width() + TITLE_MARGIN * 2);
    totalHeight += resourceSize.height() + TITLE_MARGIN;
  }
  return {0, 0, totalWidth, totalHeight};
}

void ShaderGraphNodeItem::UpdateSizeAndPositions()
{
  QRectF totalSize = CalculateTotalSize();
  QRectF titleSize = m_title->boundingRect();


  double inputPosY = titleSize.height() + TITLE_BODY_GAP;
  double outputPosY = titleSize.height() + TITLE_BODY_GAP;


  setRect(0, 0, totalSize.width(), totalSize.height());
  m_title->setPos(TITLE_MARGIN, TITLE_MARGIN);
  double titleHeight = titleSize.height();
  if (m_resourceName)
  {
    m_resourceName->setPos(TITLE_MARGIN, TITLE_MARGIN + titleSize.height() + TITLE_MARGIN);

    const QRectF &resourceSize = m_resourceName->boundingRect();
    titleHeight += resourceSize.height() + TITLE_MARGIN;
    inputPosY += resourceSize.height() + TITLE_MARGIN;
    outputPosY += resourceSize.height() + TITLE_MARGIN;
  }

  m_titleRect->setRect(TITLE_MARGIN, TITLE_MARGIN, totalSize.width() - TITLE_MARGIN * 2, titleHeight);


  for (const auto &handle: m_handles)
  {
    if (handle.IO->IsInstanceOf<csSGNodeInput>())
    {
      handle.Handle->setRect(HANDLE_MARGIN, inputPosY + HANDLE_MARGIN, HANDLE_SIZE, HANDLE_SIZE);
      handle.HandleDot->setRect(HANDLE_MARGIN + HANDLE_DOT_OFFSET,
                                inputPosY + HANDLE_MARGIN + HANDLE_DOT_OFFSET,
                                HANDLE_DOT_SIZE, HANDLE_DOT_SIZE);
      handle.Text->setPos(HANDLE_MARGIN + HANDLE_SIZE + HANDLE_TEXT_GAP, inputPosY);
      inputPosY += handle.Size.height();
    }

    if (handle.IO->IsInstanceOf<csSGNodeOutput>())
    {
      handle.Handle->setRect(totalSize.width() - HANDLE_MARGIN - HANDLE_SIZE,
                             outputPosY + HANDLE_MARGIN,
                             HANDLE_SIZE,
                             HANDLE_SIZE);
      handle.HandleDot->setRect(totalSize.width() - HANDLE_MARGIN - HANDLE_SIZE + HANDLE_DOT_OFFSET,
                                outputPosY + HANDLE_MARGIN + HANDLE_DOT_OFFSET,
                                HANDLE_DOT_SIZE,
                                HANDLE_DOT_SIZE);
      handle.Text->setPos(totalSize.width() - handle.Size.width(), outputPosY);
      outputPosY += handle.Size.height();
    }
  }

}

void ShaderGraphNodeItem::UpdateHandlesAndResource()
{
  if (m_node->IsInstanceOf<csSGResourceNode>())
  {
    csSGResourceNode* resource = m_node->Query<csSGResourceNode>();
    QString resourceName = QString(resource->GetResourceName().c_str());
    if (m_resourceName->toPlainText() != resourceName)
    {
      m_resourceName->setPlainText(resourceName);
      UpdateSizeAndPositions();
    }
  }
  
  
  for (auto &handle: m_handles)
  {
    UpdateHandle(handle);
  }
}

void ShaderGraphNodeItem::UpdateHandle(InputOutputHandle &handle)
{
  bool connected = false;
  if (csInstanceOf<csSGNodeInput>(handle.IO))
  {
    csSGNodeInput *input = csQueryClass<csSGNodeInput>(handle.IO);
    connected = input->GetSource();
  }
  else if (csInstanceOf<csSGNodeOutput>(handle.IO))
  {
    csSGNodeOutput *output = csQueryClass<csSGNodeOutput>(handle.IO);
    connected = !output->GetInputs().empty();
  }

  if (connected)
  {
    handle.HandleDot->setBrush(QBrush(QColor(255, 255, 255)));
  }
  else
  {
    handle.HandleDot->setBrush(Qt::NoBrush);
  }
}

void ShaderGraphNodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//  if (event->button() == Qt::LeftButton)
//  {
//    printf("eventPos: %.2f %.2f\n", event->pos().x(), event->pos().y());
//    if (m_titleRect->contains(event->pos()))
//    {
//      printf("  on title rect\n");
//      event->accept();
//
//      m_dragNode = true;
//      m_dragNodeMoveStartPointer = mapToScene(event->pos());
//      m_dragNodeMoveStartPosition = pos();
//      return;
//    }
//    else
//    {
//      size_t i = 0;
//      const InputOutputHandle *handle = IoHandleAt(mapToScene(event->pos()));
//      m_dragHandle = handle;
//      if (handle)
//      {
//        event->accept();
//
//        m_dragHandleMoveStartPointer = mapToScene(handle->Handle->rect().center());
////        m_dragHandleConnectionIdx = m_editorWidget->StartConnection(this, handle->IO, m_dragHandleMoveStartPointer);
//
//        if (m_dragHandleConnectionIdx)
//        {
//          handle->Handle->setBrush(QBrush(QColor(255, 255, 255)));
//        }
//        return;
//      }
//    }
//  }
//  else if (event->button() == Qt::RightButton)
//  {
//    if (!m_dragNode && !m_dragHandle)
//    {
//      const InputOutputHandle *handle = IoHandleAt(mapToScene(event->pos()));
//      if (handle && csInstanceOf<csSGNodeInput>(handle->IO))
//      {
//        csSGNodeInput *input = csQueryClass<csSGNodeInput>(handle->IO);
////        m_editorWidget->RemoveWire(input);
//      }
//
//    }
//
//
//    if (m_dragNode)
//    {
//      setPos(m_dragNodeMoveStartPosition);
////      m_editorWidget->UpdateNodeLocation(this);
//      m_dragNode = false;
//    }
//    if (m_dragHandle)
//    {
////      m_editorWidget->RollbackConnection(m_dragHandleConnectionIdx);
//      m_dragHandle->Handle->setBrush(Qt::NoBrush);
//      m_dragHandle = nullptr;
//    }
//    event->accept();
//  }
  QGraphicsRectItem::mousePressEvent(event);
}

void ShaderGraphNodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//  if (m_dragNode)
//  {
//    QPointF delta = mapToScene(event->pos()) - m_dragNodeMoveStartPointer;
//    QPointF newPos = m_dragNodeMoveStartPosition + delta;
//    setPos(newPos);
//    event->accept();
////    m_editorWidget->UpdateNodeLocation(this);
//    return;
//  }
//  else if (m_dragHandle)
//  {
//    QPointF pos = mapToScene(event->pos());
////    m_editorWidget->UpdateConnection(m_dragHandleConnectionIdx, pos);
//    event->accept();
//    return;
//  }
//

  QGraphicsRectItem::mouseMoveEvent(event);
}


void ShaderGraphNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//  if (m_dragNode)
//  {
//    m_dragNode = false;
//    event->accept();
//  }
//  if (m_dragHandle)
//  {
//    QPointF scenePos = mapToScene(event->pos());
////    m_editorWidget->CommitConnection(m_dragHandleConnectionIdx, scenePos);
//    m_dragHandleConnectionIdx = 0;
//    m_dragHandle->Handle->setBrush(Qt::NoBrush);
//    m_dragHandle = nullptr;
//    event->accept();
//  }
  QGraphicsRectItem::mouseReleaseEvent(event);
}

