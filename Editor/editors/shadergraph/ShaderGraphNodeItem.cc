//
// Created by Marcell on 08.12.2024.
//

#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

using namespace cs;

ShaderGraphNodeItem::ShaderGraphNodeItem(csSGNode *node, QGraphicsScene *scene)
    : QObject(scene)
    , m_node(nullptr)
    , m_rect(nullptr)
{
  CS_SET(m_node, node);


  m_rect = new QGraphicsRectItem ();
  m_rect->setRect(0, 0, 200, 800);
  m_rect->setBrush(QBrush(QColor(64, 64, 64, 255)));

  scene->addItem(m_rect);

  QGraphicsRectItem *titleRect = new QGraphicsRectItem(0, 0, 198, 25, m_rect);
  titleRect->setBrush(QBrush(QColor(192, 0, 0, 255)));
  titleRect->setPos(1, 1);

  QGraphicsTextItem *nodeName = new QGraphicsTextItem(node->GetName().c_str(), titleRect);
  nodeName->setDefaultTextColor(QColor(255, 255, 255, 255));

  int posX = 10;
  int posY = 35;
  for (int i=0; i<node->GetNumberOfInputs(); i++)
  {
    csSGNodeInput *input = node->GetInput(i);
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0, 0, 16, 16, m_rect);
    QGraphicsTextItem *textItem = new QGraphicsTextItem(input->GetName().c_str(), m_rect);
    ellipseItem->setPen(QPen(QColor(255, 255, 255, 255)));
//    ellipseItem->setBrush(QBrush(QColor(255, 255, 255, 255)));
    textItem->setDefaultTextColor(QColor(255, 255, 255, 255));
    ellipseItem->setPos(posX, posY);
    textItem->setPos(posX + 18, posY - 4);

    posY += 30;

  }


}



ShaderGraphNodeItem::~ShaderGraphNodeItem ()
{
  CS_RELEASE(m_node);
}

QGraphicsRectItem *ShaderGraphNodeItem::GetRoot()
{
  return m_rect;
}
