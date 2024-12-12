//
// Created by Marcell on 12.12.2024.
//

#include <editors/shadergraph/ShaderGraphGraphicsView.hh>
#include <editors/shadergraph/ShaderGraphNodePalletTreeModel.hh>
#include <csCore/csClass.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>

#include <QDataStream>
#include <QDragEnterEvent>
#include <QMimeData>

using namespace cs;

ShaderGraphGraphicsView::ShaderGraphGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{

}

void ShaderGraphGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{

  const csClass *cls = ClsFrom(event);
  if (cls && cls->IsInstanceOf<cs::csSGNode>())
  {
    event->acceptProposedAction();
  }
  QGraphicsView::dragEnterEvent(event);
}


void ShaderGraphGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{

  const csClass *cls = ClsFrom(event);
  if (cls && cls->IsInstanceOf<cs::csSGNode>())
  {
    event->acceptProposedAction();
    return;
  }
  QGraphicsView::dragMoveEvent(event);
}

void ShaderGraphGraphicsView::dropEvent(QDropEvent *event)
{
  const csClass *cls = ClsFrom(event);
  if (cls && cls->IsInstanceOf<cs::csSGNode>())
  {
    event->acceptProposedAction();
    emit dropNode(cls, event->position());
    return;
  }
  QGraphicsView::dropEvent(event);
}

const cs::csClass* ShaderGraphGraphicsView::ClsFrom (QDropEvent *event) const
{
  const QMimeData *mimeData = event->mimeData();
  if (!mimeData)
  {
    return nullptr;
  }

  if (!mimeData->hasFormat(ShaderGraphNodePalletTreeModel::s_mime.c_str()))
  {
    return nullptr;
  }
  QByteArray encodedData = mimeData->data(ShaderGraphNodePalletTreeModel::s_mime.c_str());
  QDataStream stream(&encodedData, QIODevice::ReadOnly);
  uint64_t ptr;
  stream >> ptr;

  return reinterpret_cast<const csClass*>(ptr);
}
