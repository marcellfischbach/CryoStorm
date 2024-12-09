//
// Created by Marcell on 08.12.2024.
//

#include <editors/shadergraph/ShaderGraphEditorDialog.hh>
#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include "ui_ShaderGraphEditorWidget.h"

using namespace cs;

ShaderGraphEditorDialog::ShaderGraphEditorDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint)
    , m_gui(new Ui::ShaderGraphEditorWidget)
    , m_shaderGraph(new csShaderGraph())
    , m_scene(new QGraphicsScene(this))
    , m_currentDragConnection(nullptr)
    , m_currentDragIdx(1)
{
  m_gui->setupUi(this);
  connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));

  m_gui->graph->setScene(m_scene);
  m_gui->graph->setRenderHints(QPainter::Antialiasing);
  m_scene->setBackgroundBrush(QBrush(QColor(92, 92, 92)));

  ShaderGraphNodeItem *sgNode0 = new ShaderGraphNodeItem(m_shaderGraph, this);
  ShaderGraphNodeItem *sgNode1 = new ShaderGraphNodeItem(new csSGDecomposeVec4(), this);
  m_scene->addItem(sgNode0);
  m_scene->addItem(sgNode1);


  sgNode0->setPos(0, 0);
  sgNode1->setPos(-400, 0);

  QList<int> sizes;
  sizes << 100
        << width() - 200
        << 100;
  m_gui->mainSplitter->setSizes(sizes);

}

ShaderGraphEditorDialog::~ShaderGraphEditorDialog()
{
  CS_RELEASE(m_shaderGraph);
  delete m_gui;
}


size_t ShaderGraphEditorDialog::StartConnectionInput(ShaderGraphNodeItem *item, cs::csSGNodeInput *input, QPointF pos)
{
  if (m_currentDragConnection)
  {
    return 0;
  }

  m_currentDragConnection = new QGraphicsPathItem();
  m_currentDragConnection->setPen(QPen(QColor(255, 255, 255)));

  QPainterPath path;
  path.moveTo(pos);
  path.lineTo(pos);
  m_currentDragConnection->setPath(path);

  m_scene->addItem(m_currentDragConnection);
  return ++m_currentDragIdx;
}

size_t ShaderGraphEditorDialog::StartConnectionOutput(ShaderGraphNodeItem *item,
                                                      cs::csSGNodeOutput *output,
                                                      QPointF pos)
{
  if (m_currentDragConnection)
  {
    return 0;
  }

  m_currentDragConnection = new QGraphicsPathItem();
  m_currentDragConnection->setPen(QPen(QColor(255, 255, 255), 2.0));

  QPainterPath path;
  path.moveTo(QPointF());
  path.lineTo(QPointF());
  m_currentDragConnection->setPath(path);

  m_currentDragConnection->setPos(pos);
  m_scene->addItem(m_currentDragConnection);
  return ++m_currentDragIdx;
}

void ShaderGraphEditorDialog::UpdateConnection(size_t idx, QPointF pos)
{
  if (m_currentDragIdx != idx)
  {
    return;
  }

  QPointF delta = pos - m_currentDragConnection->pos();

  QPainterPath path;
  path.moveTo(QPointF());
  path.cubicTo(QPointF(delta.x() / 2.0, 0.0),
               QPointF(delta.x() / 2.0, delta.y()),
               delta);
  m_currentDragConnection->setPath(path);
}

void ShaderGraphEditorDialog::CommitConnection(size_t idx)
{
  if (m_currentDragIdx != idx)
  {
    return;
  }

  m_scene->removeItem(m_currentDragConnection);
  delete m_currentDragConnection;
  m_currentDragConnection = nullptr;
}