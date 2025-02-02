//
// Created by Marcell on 12.12.2024.
//

#include <editors/shadergraph/ShaderGraphGraphicsView.hh>
#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <editors/shadergraph/ShaderGraphNodePalletTreeModel.hh>
#include <csCore/csClass.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>

#include <QDataStream>
#include <QDragEnterEvent>
#include <QMimeData>

using namespace cs;

ShaderGraphGraphicsView::ShaderGraphGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , m_scene(new QGraphicsScene(this))
    , m_shaderGraph(nullptr)
{
  m_scene->setBackgroundBrush(QBrush(QColor(92, 92, 92)));

  setScene(m_scene);
}

void ShaderGraphGraphicsView::SetShaderGraph(cs::csShaderGraph *shaderGraph)
{
  ClearAll();
  m_shaderGraph = shaderGraph;
  const csVector2f &sgPos = m_shaderGraph->Root()->GetPosition();
  InsertNode(m_shaderGraph->Root(), QPointF(sgPos.x, sgPos.y));
  for (int i = 0; i < shaderGraph->GetNumberOfNodes(); ++i)
  {
    csSGNode         *node = shaderGraph->GetNode(i);
    const csVector2f &pos  = node->GetPosition();
    InsertNode(node, QPointF(pos.x, pos.y));
  }

  RegenerateWires();
}


void ShaderGraphGraphicsView::mousePressEvent(QMouseEvent *event)
{

  QPointF scenePos = mapToScene(event->position().toPoint());
  if (event->button() == Qt::LeftButton)
  {
    m_leftButtonDown = true;


    ShaderGraphNodeItem *node = FindNode(scenePos);
    if (node)
    {
      if (node->IsHeader(scenePos))
      {
        m_nodeSelection.primed = true;
        m_nodeSelection.point  = event->pos();
        BeginDragSelectedNodes(scenePos);
      }
      else
      {
        csSGNodeIO *io = node->IoAt(scenePos);
        if (io)
        {
          BeginDragIO(node, io, scenePos);
        }
      }
    }
    else
    {
      ClearSelection(false);
    }
  }
  else if (event->button() == Qt::RightButton)
  {
    if (m_nodeDrag.IsDragging())
    {
      RollbackDragSelectedNodes();
      RollbackDragIO();
      UpdateWires();
    }
  }
}

void ShaderGraphGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
  QPointF scenePos = mapToScene(event->position().toPoint());
  if (m_leftButtonDown)
  {
    ShaderGraphNodeItem *node = FindNode(scenePos);
    if (node)
    {
      if (m_nodeSelection.primed)
      {
        if (node->IsHeader(scenePos))
        {
          LastMinuteSelection(scenePos, event);
        }
      }
    }
  }
  m_nodeSelection.primed = false;
  if (m_nodeDrag.IsDragging())
  {
    UpdateDragSelectedNodes(scenePos);
  }
  if (m_ioDrag.IsDragging())
  {
    UpdateDragIO(scenePos);
  }
}


void ShaderGraphGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
  const QPointF &scenePos = mapToScene(m_nodeSelection.point);
  if (event->button() == Qt::LeftButton)
  {
    m_leftButtonDown = false;
    if (m_nodeSelection.primed)
    {
      HandleSelection(scenePos, event);
      m_nodeSelection.primed = false;
    }

    if (m_nodeDrag.IsDragging())
    {
      CommitDragSelectedNodes(scenePos);
    }
    if (m_ioDrag.IsDragging())
    {
      CommitDragIO(scenePos);
    }
  }
}


void ShaderGraphGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Delete)
  {
    std::set<ShaderGraphNodeItem *> selectedNodes(m_selectedNodes);
    for (auto                       *selectedNode: selectedNodes)
    {
      DeleteNode(selectedNode);
    }
  }
}

void ShaderGraphGraphicsView::BeginDragSelectedNodes(const QPointF &scenePos)
{
  m_nodeDrag.startPosClick = scenePos;
  m_nodeDrag.startPosNodes.clear();
  for (const auto &node: m_selectedNodes)
  {
    m_nodeDrag.startPosNodes[node] = node->pos();
  }
}

std::vector<csSGNode *> ShaderGraphGraphicsView::GetSelection() const
{
  std::vector<csSGNode *> selection;

  for (auto sel: m_selectedNodes)
  {
    selection.push_back(sel->GetNode());
  }

  return selection;
}

void ShaderGraphGraphicsView::UpdateSelectedNodes()
{
  for (auto &selectedNode: m_selectedNodes)
  {
    selectedNode->UpdateHandlesAndResource();
  }
  UpdateWiresOfSelectedNodes();
}

void ShaderGraphGraphicsView::UpdateDragSelectedNodes(const QPointF &scenePos)
{
  QPointF   delta = scenePos - m_nodeDrag.startPosClick;
  for (auto it    = m_nodeDrag.startPosNodes.begin(); it != m_nodeDrag.startPosNodes.end(); it++)
  {
    ShaderGraphNodeItem *node = it->first;
    QPointF             p     = it->second + delta;
    node->setPos(p);
    node->SyncPosition();
  }

  UpdateWiresOfSelectedNodes();
}

void ShaderGraphGraphicsView::CommitDragSelectedNodes(const QPointF &scenePos)
{
  m_nodeDrag.startPosNodes.clear();
}

void ShaderGraphGraphicsView::RollbackDragSelectedNodes()
{
  for (const auto &node: m_selectedNodes)
  {
    node->setPos(m_nodeDrag.startPosNodes[node]);
    node->SyncPosition();
  }
  m_nodeDrag.startPosNodes.clear();

}

void ShaderGraphGraphicsView::BeginDragIO(ShaderGraphNodeItem *node, cs::csSGNodeIO *io, const QPointF &scenePos)
{
  // just in case something was left over
  RollbackDragIO();

  m_ioDrag.srcNode   = node;
  m_ioDrag.srcIO     = io;
  m_ioDrag.startPos  = node->ScenePosOf(io);
  m_ioDrag.graphItem = new QGraphicsPathItem();
  m_ioDrag.graphItem->setPen(QPen(QColor(255, 255, 255), 2.0));
  m_scene->addItem(m_ioDrag.graphItem);

  UpdateDragIO(scenePos);
}

void ShaderGraphGraphicsView::UpdateDragIO(const QPointF &scenePos)
{
  QPointF             dest        = scenePos;
  ShaderGraphNodeItem *node       = FindNode(scenePos);
  bool                connectable = true;
  if (node)
  {
    csSGNodeIO *io = node->IoAt(scenePos);
    if (io)
    {
      if (CanConnect(m_ioDrag.srcNode, m_ioDrag.srcIO, node, io))
      {
        connectable = true;
        QRectF rect = node->IoSceneRectAt(scenePos);
        dest = rect.center();
        m_ioDrag.dstNode = node;
        m_ioDrag.dstIO   = io;
      }
      else
      {
        connectable = false;
        m_ioDrag.dstNode = nullptr;
        m_ioDrag.dstIO   = nullptr;
      }
    }
  }
  else
  {
    connectable = false;
    m_ioDrag.dstNode = nullptr;
    m_ioDrag.dstIO   = nullptr;
  }


  QPointF      delta = dest - m_ioDrag.startPos;
  QPainterPath path;
  path.moveTo(m_ioDrag.startPos);
  path.cubicTo(QPointF(m_ioDrag.startPos.x() + delta.x() / 2.0, m_ioDrag.startPos.y()),
               QPointF(m_ioDrag.startPos.x() + delta.x() / 2.0, m_ioDrag.startPos.y() + delta.y()),
               dest
  );

  if (connectable)
  {
    m_ioDrag.graphItem->setPen(QPen(QColor(255, 255, 255), 2.0));
  }
  else
  {
    m_ioDrag.graphItem->setPen(QPen(QColor(255, 0, 0), 2.0));
  }
  m_ioDrag.graphItem->setPath(path);
}

void ShaderGraphGraphicsView::CommitDragIO(const QPointF &scenePos)
{
  ConnectDragIO();
  ClearDragIO();
}

void ShaderGraphGraphicsView::RollbackDragIO()
{
  ClearDragIO();
}

void ShaderGraphGraphicsView::ClearDragIO()
{

  m_ioDrag.srcNode = nullptr;
  m_ioDrag.srcIO   = nullptr;
  if (m_ioDrag.graphItem)
  {
    m_scene->removeItem(m_ioDrag.graphItem);
    delete m_ioDrag.graphItem;
  }
  m_ioDrag.graphItem = nullptr;
}

bool ShaderGraphGraphicsView::CanConnect(ShaderGraphNodeItem *srcNode, cs::csSGNodeIO *srcIO,
                                         ShaderGraphNodeItem *dstNode, cs::csSGNodeIO *dstIO) const
{
  if (srcNode == dstNode)
  {
    return false;
  }

  if (srcIO->GetClass() == dstIO->GetClass())
  {
    // the srcIO is the same type as the dstIO
    // meaning:
    //    connecting input -> input or output -> output
    return false;
  }

  // TODO: check if the types (vec2 vec4 , texture ... matching)

  return true;
}

bool ShaderGraphGraphicsView::IsDragIOComplete() const
{
  return m_ioDrag.srcNode && m_ioDrag.srcIO && m_ioDrag.dstNode && m_ioDrag.dstIO;
}

bool ShaderGraphGraphicsView::IsDragIOClearSource() const
{
  return csInstanceOf<csSGNodeInput>(m_ioDrag.srcIO) && !m_ioDrag.dstIO;
}

void ShaderGraphGraphicsView::ConnectDragIO()
{
  if (!IsDragIOComplete())
  {
    if (IsDragIOClearSource())
    {
      RemoveWireToInput(m_ioDrag.srcNode, csQueryClass<csSGNodeInput>(m_ioDrag.srcIO));
    }
    return;
  }

  ShaderGraphNodeItem *inputNode  = nullptr;
  ShaderGraphNodeItem *outputNode = nullptr;
  csSGNodeInput       *input      = nullptr;
  csSGNodeOutput      *output     = nullptr;
  if (csInstanceOf<csSGNodeInput>(m_ioDrag.srcIO) && csInstanceOf<csSGNodeOutput>(m_ioDrag.dstIO))
  {
    input  = csQueryClass<csSGNodeInput>(m_ioDrag.srcIO);
    output = csQueryClass<csSGNodeOutput>(m_ioDrag.dstIO);

    inputNode  = m_ioDrag.srcNode;
    outputNode = m_ioDrag.dstNode;
  }
  else if (csInstanceOf<csSGNodeOutput>(m_ioDrag.srcIO) && csInstanceOf<csSGNodeInput>(m_ioDrag.dstIO))
  {
    input      = csQueryClass<csSGNodeInput>(m_ioDrag.dstIO);
    output     = csQueryClass<csSGNodeOutput>(m_ioDrag.srcIO);
    inputNode  = m_ioDrag.dstNode;
    outputNode = m_ioDrag.srcNode;
  }

  if (!output || !input)
  {
    return;
  }

  RemoveWireToInput(inputNode, input);

  input->SetSource(output);
  output->Add(input);

  CreateWire(outputNode, output, inputNode, input);

  emit ConnectionsChanged();
}

void ShaderGraphGraphicsView::CreateWire(ShaderGraphNodeItem *outputNode,
                                         csSGNodeOutput *output,
                                         ShaderGraphNodeItem *inputNode,
                                         csSGNodeInput *input)
{
  Wire wire;
  memset(&wire, 0, sizeof(Wire));
  wire.Path          = new QGraphicsPathItem();
  wire.Source        = outputNode;
  wire.SourceIO      = output;
  wire.Destination   = inputNode;
  wire.DestinationIO = input;
  m_wires.push_back(wire);
  m_scene->addItem(wire.Path);
  UpdateWire(wire);
  UpdateWireHandles(wire);

}

void ShaderGraphGraphicsView::RemoveWireToNode(ShaderGraphNodeItem *node)
{
  for (auto &wire: std::vector<Wire>(m_wires))
  {
    if (wire.Source == node || wire.Destination == node)
    {
      RemoveWire(wire);
    }
  }
  emit ConnectionsChanged();
}


void ShaderGraphGraphicsView::RemoveWireToInput(ShaderGraphNodeItem *inputNode, cs::csSGNodeInput *input)
{
  for (auto &wire: m_wires)
  {
    if (wire.Destination == inputNode && wire.DestinationIO == input)
    {
      RemoveWire(wire);
      emit ConnectionsChanged();
      return;
    }
  }

}


void ShaderGraphGraphicsView::UpdateWires()
{
  for (auto &wire: m_wires)
  {
    UpdateWire(wire);
  }
}

void ShaderGraphGraphicsView::UpdateWiresOfSelectedNodes()
{
  for (auto &wire: m_wires)
  {
    if (std::find(m_selectedNodes.begin(), m_selectedNodes.end(), wire.Source) != m_selectedNodes.end()
        || std::find(m_selectedNodes.begin(), m_selectedNodes.end(), wire.Destination) != m_selectedNodes.end())
    {
      UpdateWire(wire);
    }
  }
}

void ShaderGraphGraphicsView::UpdateWire(ShaderGraphGraphicsView::Wire &wire)
{
  QPointF srcPoint = wire.Source->ScenePosOf(wire.SourceIO);
  QPointF dstPoint = wire.Destination->ScenePosOf(wire.DestinationIO);

  QPointF      delta = dstPoint - srcPoint;
  QPainterPath path;
  path.moveTo(srcPoint);
  path.cubicTo(QPointF(srcPoint.x() + delta.x() / 2.0, srcPoint.y()),
               QPointF(srcPoint.x() + delta.x() / 2.0, srcPoint.y() + delta.y()),
               dstPoint
  );


  wire.Path->setPath(path);
  if (CanConnect(wire.Source, wire.SourceIO, wire.Destination, wire.DestinationIO))
  {
    wire.Path->setPen(QPen(QColor(255, 255, 255), 2.0));
  }
  else
  {
    wire.Path->setPen(QPen(QColor(255, 0, 0), 2.0));
  }

}

void ShaderGraphGraphicsView::UpdateWireHandles(ShaderGraphGraphicsView::Wire &wire)
{
  if (wire.Source)
  {
    wire.Source->UpdateHandlesAndResource();
  }
  if (wire.Destination)
  {
    wire.Destination->UpdateHandlesAndResource();
  }

}

void ShaderGraphGraphicsView::RegenerateWires()
{
  for (const auto &wire: m_wires)
  {
    m_scene->removeItem(wire.Path);
    delete wire.Path;
  }
  m_wires.clear();

  for (int n = 0; n < m_shaderGraph->GetNumberOfNodes(); ++n)
  {
    csSGNode *inputNode = m_shaderGraph->GetNode(n);
    RegenerateInputWires(inputNode);
  }
  RegenerateInputWires(m_shaderGraph->Root());

  UpdateWires();
}

void ShaderGraphGraphicsView::RegenerateInputWires(cs::csSGNode *inputNode)
{
  for (int i = 0; i < inputNode->GetNumberOfInputs(); ++i)
  {
    csSGNodeInput *input = inputNode->GetInput(i);

    csSGNodeOutput *output = input->GetSource();
    if (output)
    {
      csSGNode *outputNode = output->GetNode();

      Wire wire {};
      wire.Path          = new QGraphicsPathItem();
      wire.Source        = FindNode(outputNode);
      wire.SourceIO      = output;
      wire.Destination   = FindNode(inputNode);
      wire.DestinationIO = input;
      if (!wire.IsValid())
      {
        delete wire.Path;
      }
      else
      {
        m_wires.push_back(wire);
        m_scene->addItem(wire.Path);
      }
    }
  }

}

ShaderGraphNodeItem *ShaderGraphGraphicsView::FindNode(const cs::csSGNode *node)
{
  for (const auto &item: m_nodes)
  {
    if (item->GetNode() == node)
    {
      return item;
    }
  }
  return nullptr;

}

ShaderGraphNodeItem *ShaderGraphGraphicsView::FindNode(const QPointF &scenePos)
{
  for (const auto &item: m_nodes)
  {
    const QPointF &nodePos = item->mapFromScene(scenePos);
    if (item->contains(nodePos))
    {
      return item;
    }
  }
  return nullptr;
}

void ShaderGraphGraphicsView::HandleSelection(const QPointF &scenePos, QMouseEvent *event)
{
  ShaderGraphNodeItem *node = FindNode(scenePos);
  if (!node)
  {
    ClearSelection(false);
  }
  else if (node->IsHeader(scenePos))
  {
    if ((event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) == 0)
    {
      ClearSelection(true);
      AddSelection(node);
    }
    else if (IsSelected(node))
    {
      RemoveSelection(node);
    }
    else
    {
      AddSelection(node);
    }
  }
}


void ShaderGraphGraphicsView::LastMinuteSelection(const QPointF &scenePos, QMouseEvent *event)
{
  ShaderGraphNodeItem *node = FindNode(scenePos);
  if (!node)
  {
    ClearSelection(false);
    BeginDragSelectedNodes(m_nodeDrag.startPosClick); // clears the dragging nodes
  }
  else if (!IsSelected(node))
  {
    if ((event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) == 0)
    {
      ClearSelection(true);
    }
    AddSelection(node);
    BeginDragSelectedNodes(m_nodeDrag.startPosClick);
  }
}


void ShaderGraphGraphicsView::ClearSelection(bool intermediate)
{
  for (auto node: m_selectedNodes)
  {
    node->SetSelected(false);
  }

  m_selectedNodes.clear();

  if (!intermediate)
  {
    emit SelectionChanged();
  }

}

void ShaderGraphGraphicsView::AddSelection(ShaderGraphNodeItem *node)
{
  node->SetSelected(true);

  m_selectedNodes.insert(node);

  emit SelectionChanged();
}


void ShaderGraphGraphicsView::RemoveSelection(ShaderGraphNodeItem *node)
{
  node->SetSelected(false);

  auto it = std::find(m_selectedNodes.begin(), m_selectedNodes.end(), node);
  if (it != m_selectedNodes.end())
  {
    m_selectedNodes.erase(it);
  }

  emit SelectionChanged();
}


bool ShaderGraphGraphicsView::IsSelected(ShaderGraphNodeItem *node) const
{
  return std::find(m_selectedNodes.begin(), m_selectedNodes.end(), node) != m_selectedNodes.end();
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
    InsertNode(cls, event->position());
    return;
  }
  QGraphicsView::dropEvent(event);
}

const cs::csClass *ShaderGraphGraphicsView::ClsFrom(QDropEvent *event) const
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
  QByteArray  encodedData = mimeData->data(ShaderGraphNodePalletTreeModel::s_mime.c_str());
  QDataStream stream(&encodedData, QIODevice::ReadOnly);
  uint64_t    ptr;
  stream >> ptr;

  return reinterpret_cast<const csClass *>(ptr);
}


std::string extract_node_key(const csSGNode *node)
{
  std::string name = node->GetName();
  for (size_t i    = 0; i < name.size(); i++)
  {
    char ch = name[i];
    if (i != 0 && ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
    {
      continue;
    }
    name[i] = '_';
  }
  return name;
}

size_t ShaderGraphGraphicsView::GetFreeNodeId() const
{
  for (size_t i = 0;; i++)
  {
    std::string idString = "id_" + std::to_string(i);
    if (!m_shaderGraph->GetNode(idString))
    {
      return i;
    }
  }
}

void ShaderGraphGraphicsView::InsertNode(const csClass *cls, const QPointF &position)
{
  try
  {
    if (!cls->IsInstanceOf<csSGNode>())
    {
      return;
    }

    csRef<csSGNode> tmpNode = cls->CreateInstance<csSGNode>();
    if (!tmpNode)
    {
      printf ("Unable to create node: %s\n", cls->GetName().c_str());
      fflush(stdout);
      return;
    }
    std::string     key     = "id_" + std::to_string(GetFreeNodeId());

    csRef<csSGNode> node;
    if (cls->IsInstanceOf<csSGResourceNode>())
    {
      node = m_shaderGraph->AddResource(cls, key, tmpNode->GetName());
    }
    else
    {
      node = m_shaderGraph->Add(cls, key);
    }
    const QPointF   &scenePos = mapToScene(position.toPoint());
    InsertNode(node, scenePos);
  }
  catch (std::exception &e)
  {

  }
}

void ShaderGraphGraphicsView::InsertNode(cs::csSGNode *node, const QPointF &scenePos)
{
  ShaderGraphNodeItem *nodeItem = new ShaderGraphNodeItem(node, nullptr);
  nodeItem->setPos(scenePos);
  nodeItem->SyncPosition();

  m_nodes.push_back(nodeItem);
  m_scene->addItem(nodeItem);
}

void ShaderGraphGraphicsView::ClearAll()
{
  for (auto &wire: m_wires)
  {
    RemoveWire(wire);
  }


  for (auto node: m_nodes)
  {
    DeleteNode(node);
  }

}

std::string name(ShaderGraphNodeItem *node, cs::csSGNodeIO *io)
{
  std::string name;
  name += node->GetNode()->GetName().c_str();
  name += "@";
  name += io->GetName();
  return name;
}

void ShaderGraphGraphicsView::RemoveWire(ShaderGraphGraphicsView::Wire &wire)
{

  const auto it = std::find(m_wires.begin(), m_wires.end(), wire);

  m_scene->removeItem(wire.Path);


  wire.SourceIO->Remove(wire.DestinationIO);
  wire.DestinationIO->SetSource(nullptr);
  UpdateWireHandles(wire);

  delete wire.Path;

  if (it != m_wires.end())
  {
    m_wires.erase(it);
  }

}

void ShaderGraphGraphicsView::DeleteNode(ShaderGraphNodeItem *node)
{
  RemoveWireToNode(node);

  {
    auto it = std::find(m_nodes.begin(), m_nodes.end(), node);
    if (it != m_nodes.end())
    {
      m_nodes.erase(it);
    }
  }

  {
    auto it = std::find(m_selectedNodes.begin(), m_selectedNodes.end(), node);
    if (it != m_selectedNodes.end())
    {
      m_selectedNodes.erase(it);
    }
  }

  m_shaderGraph->Remove(node->GetNode());

  m_scene->removeItem(node);
  delete node;


  RegenerateWires();
}

bool ShaderGraphGraphicsView::Wire::operator==(const ShaderGraphGraphicsView::Wire &wire) const
{
  return wire.Path == Path
         && wire.Source == Source
         && wire.SourceIO.raw() == SourceIO
         && wire.Destination == Destination
         && wire.DestinationIO.raw() == DestinationIO;
}
bool ShaderGraphGraphicsView::Wire::IsValid() const
{
  return Path
         && Source
         && SourceIO
         && Destination
         && DestinationIO;
}
