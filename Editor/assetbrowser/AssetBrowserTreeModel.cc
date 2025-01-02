//
// Created by Marcell on 16.09.2024.
//

#include <assetbrowser/AssetBrowserTreeModel.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csFileSystemArchive.hh>
#include <QDir>
#include <QFileInfo>


AssetBrowserTreeModel::AssetBrowserTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

void AssetBrowserTreeModel::Reload()
{
  for (auto archive: m_archives)
  {
    delete archive;
  }
  m_archives.clear();

  for (auto archive: cs::csVFS::Get()->GetArchives())
  {


    LoadArchive(archive);
  }
}

void AssetBrowserTreeModel::LoadArchive(const cs::iArchive *archive)
{
  const cs::csFileSystemArchive *fsArchive = archive->Query<cs::csFileSystemArchive>();
  if (!fsArchive)
  {
    return;
  }
  const std::string &root = fsArchive->GetRootPath();

  QFileInfo fileInfo(root.c_str());
  if (!fileInfo.isDir())
  {
    return;
  }


  ArchiveItem *item = new ArchiveItem(fsArchive);
  item->Name = fsArchive->GetName();
  item->FullPath = root;
  m_archives.push_back(item);

  QDir dir(root.c_str());
  QStringList dirs = dir.entryList(QStringList(), QDir::Filter::Dirs | QDir::NoDotAndDotDot);

  for (auto childDir: dirs)
  {
    LoadPath(item, dir, childDir);
  }

  return;
}

void AssetBrowserTreeModel::LoadPath(Item *parent, QDir parentDir, QString childDir)
{
  QString path = parentDir.filePath(childDir);
  QFileInfo fileInfo(path);
  if (!fileInfo.isDir())
  {
    return;
  }

  PathItem *item = new PathItem();
  item->Name = childDir.toLatin1().data();
  item->Parent = parent;
  item->FullPath = path.toLatin1().data();
  parent->Children.push_back(item);


  QDir dir(path);
  QStringList dirs = dir.entryList(QStringList(), QDir::Filter::Dirs | QDir::NoDotAndDotDot);
  for (auto childDir: dirs)
  {
    LoadPath(item, dir, childDir);
  }

}

std::string AssetBrowserTreeModel::GetPath(const QModelIndex &index) const
{
  const Item *item = GetItem(index);
  if (!item)
  {
    return {};
  }


  return item->FullPath;
}

const AssetBrowserTreeModel::Item *AssetBrowserTreeModel::GetItem(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return {};
  }

  return reinterpret_cast<const Item *>(index.internalPointer());
}

std::string AssetBrowserTreeModel::ConstructArchivePath(const QModelIndex &index) const
{
  std::string path;
  const Item *item = GetItem(index);
  while (item)
  {
    if (item->IsArchive())
    {
      break;
    }
    path = item->Name + "/" + path;
    item = item->Parent;
  }
  return path;

}

const cs::csFileSystemArchive *AssetBrowserTreeModel::ExtractArchive(const QModelIndex &index) const
{
  const Item *item = GetItem(index);
  while (item)
  {
    if (item->IsArchive())
    {
      const ArchiveItem *archiveItem = dynamic_cast<const ArchiveItem *>(item);
      return archiveItem->m_fsArchive;
    }
    item = item->Parent;
  }
  return nullptr;
}

QModelIndex AssetBrowserTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    Item *item = reinterpret_cast<Item *>(parent.internalPointer());
    if (row >= item->Children.size())
    {
      return QModelIndex();
    }

    return createIndex(row, column, item->Children[row]);
  }

  else
  {
    if (row >= m_archives.size())
    {
      return QModelIndex();
    }

    return createIndex(row, column, m_archives[row]);
  }
}

QModelIndex AssetBrowserTreeModel::parent(const QModelIndex &child) const
{
  Item *childItem = reinterpret_cast<Item *>(child.internalPointer());

  if (!childItem)
  {
    return QModelIndex();
  }

  Item *parentItem = childItem->Parent;
  if (!parentItem)
  {
    return QModelIndex();
  }

  return createIndex(SelfIndex(parentItem), 0, parentItem);
}

int AssetBrowserTreeModel::rowCount(const QModelIndex &parent) const
{

  if (!parent.isValid())
  {

    // this is the root
    return m_archives.size();
  }

  Item *parentItem = reinterpret_cast<Item *>(parent.internalPointer());

  return parentItem->Children.size();
}


int AssetBrowserTreeModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}
QVariant AssetBrowserTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  Item *item = reinterpret_cast<Item *>(index.internalPointer());
  if (role == Qt::DisplayRole)
  {

    switch (index.column())
    {
      case 0:
        return QVariant(QString(item->Name.c_str()));
    }

  }

  return QVariant();
}


int AssetBrowserTreeModel::IndexOf(AssetBrowserTreeModel::Item *parent, AssetBrowserTreeModel::Item *child) const
{
  for (int i = 0; i < parent->Children.size(); i++)
  {
    if (parent->Children[i] == child)
    {
      return i;
    }
  }
  return -1;
}

int AssetBrowserTreeModel::SelfIndex(AssetBrowserTreeModel::Item *item) const
{
  if (item->Parent)
  {
    return IndexOf(item->Parent, item);
  }
  for (int i = 0; i < m_archives.size(); i++)
  {
    if (m_archives[i] == item)
    {
      return i;
    }
  }
  return -1;
}


AssetBrowserTreeModel::ArchiveItem::ArchiveItem(const cs::csFileSystemArchive *fsArchive)
    : m_fsArchive(fsArchive)
{

}