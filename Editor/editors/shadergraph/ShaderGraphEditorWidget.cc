//
// Created by Marcell on 08.12.2024.
//

#include <editors/shadergraph/ShaderGraphEditorWidget.hh>
#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <editors/shadergraph/ShaderGraphNodePalletTreeModel.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include "ui_ShaderGraphEditorWidget.h"

using namespace cs;

ShaderGraphEditorWidget::ShaderGraphEditorWidget(QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint)
    , m_gui(new Ui::ShaderGraphEditorWidget)
    , m_shaderGraph(new csShaderGraph())
    , m_palletModel(new ShaderGraphNodePalletTreeModel())
    , m_scene(new QGraphicsScene(this))
    , m_currentDragConnection(nullptr)
    , m_currentDragIdx(1)
{
  m_gui->setupUi(this);
  m_gui->pallet->setModel(m_palletModel);
  connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));


  m_gui->graph->setScene(m_scene);
  m_gui->graph->setRenderHints(QPainter::Antialiasing);
  m_scene->setBackgroundBrush(QBrush(QColor(92, 92, 92)));


  InsertNode(m_shaderGraph, QPointF(0.0, 0.0));


  QList<int> sizes;
  sizes << 100
        << width() - 200
        << 100;
  m_gui->mainSplitter->setSizes(sizes);

}

ShaderGraphEditorWidget::~ShaderGraphEditorWidget()
{
  CS_RELEASE(m_shaderGraph);
  delete m_gui;
}


void ShaderGraphEditorWidget::keyPressEvent(QKeyEvent *event)
{
  QDialog::keyPressEvent(event);
  printf ("Key pressed\n");
}

void ShaderGraphEditorWidget::keyReleaseEvent(QKeyEvent *event)
{
  QDialog::keyReleaseEvent(event);
  printf ("Key release\n");

}

void ShaderGraphEditorWidget::on_graph_dropNode(const cs::csClass *cls, const QPointF &pos)
{
  printf ("Drop at: %.2f %.2f\n", pos.x(), pos.y());
  const QPointF &point = m_gui->graph->mapToScene(pos.toPoint());
  csSGNode *node = cls->CreateInstance<csSGNode>();
  if (!node)
  {
    return;
  }

  InsertNode(node, point);
}


void ShaderGraphEditorWidget::InsertNode(cs::csSGNode *node, const QPointF &scenePos)
{
  ShaderGraphNodeItem *nodeItem = new ShaderGraphNodeItem(node, this);
  m_scene->addItem(nodeItem);
  m_nodes.push_back(nodeItem);
  nodeItem->setPos(scenePos);

}

size_t ShaderGraphEditorWidget::StartConnection(ShaderGraphNodeItem *item, cs::csSGNodeIO *io, QPointF pos)
{
  if (m_currentDragConnection)
  {
    return 0;
  }

  m_currentDragConnection = new QGraphicsPathItem();
  m_currentDragConnection->setPen(QPen(QColor(255, 255, 255)));

  m_wireStartNode = item;
  m_wireStartIO = io;

  QPainterPath path;
  path.moveTo(QPointF());
  path.lineTo(QPointF());
  m_currentDragConnection->setPath(path);
  m_currentDragConnection->setPos(pos);

  m_scene->addItem(m_currentDragConnection);
  return ++m_currentDragIdx;
}

ShaderGraphNodeItem *ShaderGraphEditorWidget::At(const QPointF &scenePoint)
{
  for (auto node: m_nodes)
  {
    QPointF local = node->mapFromScene(scenePoint);
    if (node->contains(local))
    {
      return node;
    }
  }
  return nullptr;
}

static bool is_valid_binding(cs::csSGNodeIO *io0, cs::csSGNodeIO *io1)
{
  return io0->GetNode() != io1->GetNode()
         && (io0->IsInstanceOf<cs::csSGNodeInput>() && io1->IsInstanceOf<cs::csSGNodeOutput>()
             || io0->IsInstanceOf<cs::csSGNodeOutput>() && io1->IsInstanceOf<cs::csSGNodeInput>()
         );
}

void ShaderGraphEditorWidget::UpdateConnection(size_t idx, QPointF scenePos)
{
  if (m_currentDragIdx != idx)
  {
    return;
  }


  ShaderGraphNodeItem *nodeItem = At(scenePos);
  if (nodeItem)
  {
    cs::csSGNodeIO *io = nodeItem->IoAt(scenePos);
    if (io)
    {

      if (is_valid_binding(m_wireStartIO, io))
      {
        QRectF rect = nodeItem->IoRectAt(scenePos);
        if (rect.isValid())
        {
          scenePos = rect.center();
        }
      }
    }
  }

  QPointF delta = scenePos - m_currentDragConnection->pos();

  QPainterPath path;
  path.moveTo(QPointF());
  path.cubicTo(QPointF(delta.x() / 2.0, 0.0),
               QPointF(delta.x() / 2.0, delta.y()),
               delta);
  m_currentDragConnection->setPath(path);
}

void ShaderGraphEditorWidget::UpdateNodeLocation(ShaderGraphNodeItem *node)
{
  for (auto &wire: m_wires)
  {
    if (wire.Source == node || wire.Destination == node)
    {
      UpdateWire(wire);
    }
  }
}


void ShaderGraphEditorWidget::UpdateWires()
{
  for (auto &wire: m_wires)
  {
    UpdateWire(wire);
  }
}

void ShaderGraphEditorWidget::UpdateWire(ShaderGraphEditorWidget::Wire &wire)
{
  if (!wire.Path)
  {
    wire.Path = new QGraphicsPathItem();
    wire.Path->setPen(QPen(QColor(255, 255, 255), 2.0));
    m_scene->addItem(wire.Path);
  }

  QPointF P0 = wire.Source->ScenePosOf(wire.SourceIO);
  QPointF P1 = wire.Destination->ScenePosOf(wire.DestinationIO);
  QPointF delta = P1 - P0;

  QPainterPath path;
  path.moveTo(P0);
  path.cubicTo(P0 + QPointF(delta.x() / 2.0, 0.0),
               P0 + QPointF(delta.x() / 2.0, delta.y()),
               P1);
  wire.Path->setPath(path);
}

void ShaderGraphEditorWidget::RemoveWire(cs::csSGNodeInput *input)
{
  for (auto it = m_wires.begin(); it != m_wires.end(); it++)
  {
    Wire& wire = *it;
    if (wire.DestinationIO == input)
    {
      wire.DestinationIO->SetSource(nullptr);
      wire.SourceIO->Remove(wire.DestinationIO);

      m_scene->removeItem(wire.Path);
      delete wire.Path;
      m_wires.erase(it);

      wire.Source->UpdateHandles();
      wire.Destination->UpdateHandles();
      return;
    }
  }
}


void ShaderGraphEditorWidget::WireIOs(ShaderGraphNodeItem *node0,
                                      cs::csSGNodeIO *io0,
                                      ShaderGraphNodeItem *node1,
                                      cs::csSGNodeIO *io1)
{
  if (is_valid_binding(io0, io1))
  {
    if (io0->IsInstanceOf<cs::csSGNodeInput>())
    {
      cs::csSGNodeInput *input = csQueryClass<cs::csSGNodeInput>(io0);
      RemoveWire(input);

      cs::csSGNodeOutput *output = csQueryClass<cs::csSGNodeOutput>(io1);
      input->SetSource(output);
      output->Add(input);

      Wire &wire = m_wires.emplace_back(nullptr, node1, output, node0, input);
      UpdateWire(wire);
    }

    if (io1->IsInstanceOf<cs::csSGNodeInput>())
    {
      cs::csSGNodeInput *input = csQueryClass<cs::csSGNodeInput>(io1);
      RemoveWire(input);

      cs::csSGNodeOutput *output = csQueryClass<cs::csSGNodeOutput>(io0);
      input->SetSource(output);
      output->Add(input);

      Wire &wire = m_wires.emplace_back(nullptr, node0, output, node1, input);
      UpdateWire(wire);
    }

    node0->UpdateHandles();
    node1->UpdateHandles();
  }
}

static std::string name(cs::csSGNodeIO *io)
{
  return io->GetNode()->GetName() + "[" + io->GetName() + "]";
}

void ShaderGraphEditorWidget::CommitConnection(size_t idx, QPointF scenePos)
{
  if (m_currentDragIdx != idx)
  {
    return;
  }

  if (m_wireStartIO)
  {
    ShaderGraphNodeItem *nodeItem = At(scenePos);
    if (nodeItem)
    {
      cs::csSGNodeIO *io = nodeItem->IoAt(scenePos);
      if (io)
      {

        if (is_valid_binding(m_wireStartIO, io))
        {
          WireIOs(m_wireStartNode, m_wireStartIO,
               nodeItem, io);
          printf("Bind %s -> %s\n",
                 name(m_wireStartIO).c_str(),
                 name(io).c_str());
        }
      }
    }
  }

  m_scene->removeItem(m_currentDragConnection);
  delete m_currentDragConnection;
  m_currentDragConnection = nullptr;
  m_currentDragIdx = 0;
}

void ShaderGraphEditorWidget::RollbackConnection(size_t idx)
{
  if (m_currentDragIdx != idx)
  {
    return;
  }

  m_scene->removeItem(m_currentDragConnection);
  delete m_currentDragConnection;
  m_currentDragConnection = nullptr;
  m_currentDragIdx = 0;
}