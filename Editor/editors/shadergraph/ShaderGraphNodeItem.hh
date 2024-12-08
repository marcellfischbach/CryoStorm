//
// Created by Marcell on 08.12.2024.
//

#pragma once

#include <QObject>


class QGraphicsScene;
class QGraphicsRectItem;

namespace cs
{
class csSGNode;
}



class ShaderGraphNodeItem : public QObject
{
  Q_OBJECT
public:
  ShaderGraphNodeItem (cs::csSGNode *node, QGraphicsScene *scene);
  ~ShaderGraphNodeItem() override;

  QGraphicsRectItem *GetRoot();

private:
  cs::csSGNode *m_node;
  QGraphicsRectItem *m_rect;

};
