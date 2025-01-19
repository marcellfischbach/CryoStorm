//
// Created by Marcell on 12.12.2024.
//

#include <editors/shadergraph/ShaderGraphNodePalletTreeModel.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <csCore/csClassRegistry.hh>

#include <QIODevice>
#include <QMimeData>

using namespace cs;


const std::string ShaderGraphNodePalletTreeModel::s_mime = "application/sg-node-pallet-cls-ptr";

ShaderGraphNodePalletTreeModel::ShaderGraphNodePalletTreeModel()
{
  const std::vector<const csClass *> &allClasses = csClassRegistry::Get()->GetAllClasses();
  for (const csClass *cls: allClasses)
  {
    if (cls->IsInstanceOf<csSGNode>() && !cls->IsInstanceOf<csShaderGraph>())
    {
      try
      {
        // just create a temporary node to acquire the name
        csRef<csSGNode> node = cls->CreateInstance<csSGNode>();
        if (node)
        {
          m_nodes.emplace_back(cls, QString(node->GetName().c_str()));
        }
      }
      catch (const csInstantiationException &exception)
      {
        // this can happen
      }
    }
  }
}

QModelIndex ShaderGraphNodePalletTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return QModelIndex();
  }

  return createIndex(row, column, row);
}

QModelIndex ShaderGraphNodePalletTreeModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int ShaderGraphNodePalletTreeModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return 0;
  }
  return m_nodes.size();
}

int ShaderGraphNodePalletTreeModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

bool ShaderGraphNodePalletTreeModel::hasChildren(const QModelIndex &parent) const
{
  if (!parent.isValid())
  {
    return m_nodes.size() > 0;
  }
  return false;
}

QVariant ShaderGraphNodePalletTreeModel::data(const QModelIndex &index, int role) const
{
  size_t idx = index.row();
  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  if (idx >= m_nodes.size())
  {
    return QVariant();
  }

  return QVariant(m_nodes[idx].Name);
}

Qt::DropActions ShaderGraphNodePalletTreeModel::supportedDragActions() const
{
  return Qt::DropAction::ActionMask;
}

Qt::ItemFlags ShaderGraphNodePalletTreeModel::flags(const QModelIndex &index) const
{

  if (index.isValid())
  {
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    return Qt::ItemIsDragEnabled | defaultFlags;
  }
  return QAbstractItemModel::flags(index);
}

QStringList ShaderGraphNodePalletTreeModel::mimeTypes() const
{
  return QStringList() << s_mime.c_str();
}

QMimeData *ShaderGraphNodePalletTreeModel::mimeData(const QModelIndexList &indexes) const
{
  QMimeData *mimeData = new QMimeData();
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);
  for (const auto &index: indexes)
  {
    if (index.isValid() && index.row() < m_nodes.size())
    {
      stream << reinterpret_cast<uint64_t>(m_nodes[index.row()].Cls);
    }
  }
  mimeData->setData(s_mime.c_str(), encodedData);
  return mimeData;
}