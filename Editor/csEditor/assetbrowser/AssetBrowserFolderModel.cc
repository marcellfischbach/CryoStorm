//
// Created by mcel on 17.09.2024.
//

#include <csEditor/assetbrowser/AssetBrowserFolderModel.hh>
#include <QModelIndex>
#include <QDir>
#include <QFileInfo>


AssetBrowserFolderModel::AssetBrowserFolderModel(QObject *parent)
: QAbstractItemModel(parent)
{

}

void AssetBrowserFolderModel::SetFolder(const std::string &folder)
{
  beginResetModel();
  m_folder = folder;
  LoadFolder();

  endResetModel();
}

const std::string& AssetBrowserFolderModel::GetFolder() const
{
  return m_folder;
}

std::string AssetBrowserFolderModel::GetName(const QModelIndex &index) const
{
  if (!index.isValid() || index.row() >= m_items.size())
  {
    return std::string();
  }

  return m_items[index.row()].Name;
}

void AssetBrowserFolderModel::LoadFolder()
{
  m_items.clear();
  QFileInfo info(m_folder.c_str());
  if (!info.isDir())
  {
    return;
  }


  QDir              dir(m_folder.c_str());
  const QStringList &files = dir.entryList(QDir::Filter::Files, QDir::SortFlag::Name);
  for (auto &fileName: files)
  {
    Item item {std::string(fileName.toLatin1().data())};
    m_items.push_back(item);
  }
}

QModelIndex AssetBrowserFolderModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return QModelIndex();
  }

  if (row >= m_items.size())
  {
    return QModelIndex();
  }

  return createIndex(row, column, nullptr);
}

QModelIndex AssetBrowserFolderModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int AssetBrowserFolderModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return 0;
  }
  return m_items.size();
}

int AssetBrowserFolderModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant AssetBrowserFolderModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() >= m_items.size())
  {
    return QVariant();
  }

  if (role == Qt::DisplayRole)
  {
    const Item &item = m_items[index.row()];
    switch (index.column())
    {
      case 0:
        return QVariant(item.Name.c_str());
    }
  }
  return QVariant();
}
