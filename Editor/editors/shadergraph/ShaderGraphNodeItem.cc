//
// Created by Marcell on 08.12.2024.
//

#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <editors/shadergraph/ShaderGraphEditorDialog.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

using namespace cs;

static const double HANDLE_SIZE     = 16.0;
static const double HANDLE_MARGIN   = 4.0;
static const double HANDLE_TEXT_GAP = 2.0;

static const double TITLE_MARGIN     = 1.0;
static const double TITLE_BODY_GAP   = 4.0;
static const double INPUT_OUTPUT_GAP = 16.0;

ShaderGraphNodeItem::ShaderGraphNodeItem(csSGNode *node, ShaderGraphEditorDialog* dlg)
    : QGraphicsRectItem()
    , m_node(nullptr)
    , m_dlg(dlg)
    , m_dragNode(false)
    , m_dragHandle(false)
{
  CS_SET(m_node, node);
  setData(CUSTOM_DATA_IS_NODE_ITEM, true);

  GenerateSurroundingRect();
  GenerateInputsAndOutputs();
  CalculateSizes();

  UpdateSizeAndPositions();
}


void ShaderGraphNodeItem::GenerateSurroundingRect()
{

  setBrush(QBrush(QColor(32, 32, 32, 255)));
  setPen(QPen(QColor(32, 32, 32, 255)));

  m_titleRect = new QGraphicsRectItem(0, 0, 198, 25, this);
  m_titleRect->setBrush(QBrush(QColor(192, 0, 0, 255)));
  m_titleRect->setPen(QPen(QColor(192, 0, 0, 255)));

  m_title = new QGraphicsTextItem(m_node->GetName().c_str(), m_titleRect);
  m_title->setDefaultTextColor(QColor(255, 255, 255, 255));
  m_title->setPos(TITLE_MARGIN, TITLE_MARGIN);
}

void ShaderGraphNodeItem::GenerateInputsAndOutputs()
{
  QColor   whiteColor(255, 255, 255);
  for (int i = 0; i < m_node->GetNumberOfInputs(); i++)
  {
    csSGNodeInput        *input       = m_node->GetInput(i);
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0, 0, HANDLE_SIZE, HANDLE_SIZE, this);
    QGraphicsTextItem    *textItem    = new QGraphicsTextItem(input->GetName().c_str(), this);
    ellipseItem->setPen(QPen(whiteColor));
    textItem->setDefaultTextColor(whiteColor);
    m_handles.emplace_back(input, nullptr, ellipseItem, textItem, QRectF());
  }
  for (int i = 0; i < m_node->GetNumberOfOutputs(); i++)
  {
    csSGNodeOutput       *output      = m_node->GetOutput(i);
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0, 0, HANDLE_SIZE, HANDLE_SIZE, this);
    QGraphicsTextItem    *textItem    = new QGraphicsTextItem(output->GetName().c_str(), this);
    ellipseItem->setPen(QPen(whiteColor));
    textItem->setDefaultTextColor(whiteColor);
    m_handles.emplace_back(nullptr, output, ellipseItem, textItem, QRectF());
  }
}

void ShaderGraphNodeItem::CalculateSizes()
{
  for (auto &handle: m_handles)
  {
    double handleWidth  = HANDLE_SIZE + HANDLE_MARGIN + HANDLE_TEXT_GAP;
    double handleHeight = HANDLE_SIZE + HANDLE_MARGIN; // only one margin because the margin on the other side comes
    // from the handle item blow or above

    const QRectF &textSize = handle.Text->boundingRect();
    handleWidth += textSize.width();
    handleHeight           = std::max(handleHeight, textSize.height());

    handle.Size = QRectF(0.0, 0.0, handleWidth, handleHeight);
  }
}


QRectF ShaderGraphNodeItem::CalculateTotalSize()
{
  QRectF          titleSize    = m_title->boundingRect();
  double          inputWidth   = 0.0;
  double          inputHeight  = 0.0;
  double          outputWidth  = 0.0;
  double          outputHeight = 0.0;
  for (const auto &item: m_handles)
  {
    if (item.Input)
    {
      inputWidth = std::max(inputWidth, item.Size.width());
      inputHeight += item.Size.height();
    }
    if (item.Output)
    {
      outputWidth = std::max(outputWidth, item.Size.width());
      outputHeight += item.Size.height();
    }
  }

  double totalWidth  = inputWidth + outputWidth + INPUT_OUTPUT_GAP;
  double totalHeight = std::max(inputHeight, outputHeight);

  totalWidth = std::max(totalWidth, titleSize.width() + TITLE_MARGIN * 2);
  totalHeight += titleSize.height() + TITLE_BODY_GAP + 2;
  return {0, 0, totalWidth, totalHeight};
}

void ShaderGraphNodeItem::UpdateSizeAndPositions()
{
  QRectF totalSize = CalculateTotalSize();
  QRectF titleSize = m_title->boundingRect();


  double inputPosY  = titleSize.height() + TITLE_BODY_GAP;
  double outputPosY = titleSize.height() + TITLE_BODY_GAP;


  setRect(0, 0, totalSize.width(), totalSize.height());
  m_titleRect->setRect(TITLE_MARGIN, TITLE_MARGIN, totalSize.width() - TITLE_MARGIN * 2, titleSize.height());

  for (const auto &handle: m_handles)
  {
    if (handle.Input)
    {
      handle.Handle->setRect(HANDLE_MARGIN, inputPosY + HANDLE_MARGIN, HANDLE_SIZE, HANDLE_SIZE);
      handle.Text->setPos(HANDLE_MARGIN + HANDLE_SIZE + HANDLE_TEXT_GAP, inputPosY);
      inputPosY += handle.Size.height();
    }

    if (handle.Output)
    {
      handle.Handle->setRect(totalSize.width() - HANDLE_MARGIN - HANDLE_SIZE,
                             outputPosY + HANDLE_MARGIN,
                             HANDLE_SIZE,
                             HANDLE_SIZE);
      handle.Text->setPos(totalSize.width() - handle.Size.width(), outputPosY);
      outputPosY += handle.Size.height();
    }
  }

}

void ShaderGraphNodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  printf ("eventPos: %.2f %.2f\n", event->pos().x(), event->pos().y());
  if (m_titleRect->contains(event->pos()))
  {
    printf ("  on title rect\n");
    event->accept();

    m_dragNode                 = true;
    m_dragNodeMoveStartPointer  = mapToScene(event->pos());
    m_dragNodeMoveStartPosition = pos();
    return;
  }
  else
  {
    size_t i=0;
    for (const auto &handle: m_handles)
    {
      QPointF pos = mapToItem(handle.Handle, event->pos());
      if (handle.Handle->contains(pos))
      {
        event->accept();

        m_dragHandleIdx = i;
        m_dragHandle = true;
        m_dragHandleMoveStartPointer = mapToScene(handle.Handle->rect().center());

        if (handle.Input)
        {
          m_dragHandleConnectionIdx = m_dlg->StartConnectionInput(this, handle.Input, m_dragNodeMoveStartPointer);
        }
        else if (handle.Output)
        {
          m_dragHandleConnectionIdx = m_dlg->StartConnectionOutput(this, handle.Output, m_dragHandleMoveStartPointer);
        }

        if (m_dragHandleConnectionIdx)
        {
          handle.Handle->setBrush(QBrush(QColor(255, 255, 255)));
        }
        return;
      }
      i++;
    }
  }
  QGraphicsRectItem::mousePressEvent(event);
}

void ShaderGraphNodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (m_dragNode)
  {
    QPointF delta = mapToScene(event->pos()) - m_dragNodeMoveStartPointer;
    QPointF newPos = m_dragNodeMoveStartPosition + delta;
    setPos(newPos);
    event->accept();
    return;
  }
  else if (m_dragHandle)
  {
    QPointF pos = mapToScene(event->pos());
    m_dlg->UpdateConnection(m_dragHandleConnectionIdx, pos);
    event->accept();
    return;
  }


  QGraphicsRectItem::mouseMoveEvent(event);
}


void ShaderGraphNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (m_dragNode)
  {
    m_dragNode = false;
    event->accept();
  }
  if (m_dragHandle)
  {
    m_dragHandle = false;
    m_dlg->CommitConnection(m_dragHandleConnectionIdx);
    m_dragHandleConnectionIdx = 0;

    if (m_dragHandleIdx < m_handles.size())
    {
      m_handles[m_dragHandleIdx].Handle->setBrush(Qt::NoBrush);
    }

    event->accept();
  }
  QGraphicsRectItem::mouseReleaseEvent(event);
}



ShaderGraphNodeItem::~ShaderGraphNodeItem()
{
  CS_RELEASE(m_node);
}
