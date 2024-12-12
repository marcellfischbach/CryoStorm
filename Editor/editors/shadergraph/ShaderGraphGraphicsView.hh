//
// Created by Marcell on 12.12.2024.
//

#pragma once

#include <QGraphicsView>

namespace cs
{
class csClass;
}

class ShaderGraphGraphicsView : public QGraphicsView
{
  Q_OBJECT
public:
  ShaderGraphGraphicsView(QWidget* parent = nullptr);

  void dragEnterEvent (QDragEnterEvent *event) override;
  void dragMoveEvent (QDragMoveEvent *event) override;
  void dropEvent (QDropEvent *event) override;

signals:
  void dropNode (const cs::csClass* cls, const QPointF &pos);

private:
  const cs::csClass* ClsFrom (QDropEvent *event) const;

};
