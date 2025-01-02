#pragma once

#include <QAbstractTableModel>
#include <csCore/resource/csVFS.hh>
#include <QDir>

namespace cs
{
class csFileSystemArchive;
};

class AssetBrowserTreeModel : public QAbstractItemModel
{
public:
  AssetBrowserTreeModel(QObject *parent);

  void Reload();

  std::string GetPath(const QModelIndex &index) const;

  struct Item;
  const Item* GetItem (const QModelIndex &index) const;

  std::string ConstructArchivePath(const QModelIndex &index) const;
  const cs::csFileSystemArchive *ExtractArchive(const QModelIndex &index) const;


public:

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;


public:

  struct PathItem;
  struct Item
  {
    Item *Parent = nullptr;
    std::vector<PathItem *> Children;
    std::string Name;
    std::string FullPath;


    virtual ~Item()
    {

      for (auto child: Children)
      {
        delete child;
      }
      Children.clear();
    }

    virtual bool IsPath () const
    {
      return false;
    }
    virtual bool IsArchive () const
    {
      return false;
    }

  };

  struct ArchiveItem : public Item
  {
    ArchiveItem(const cs::csFileSystemArchive *fsArchive);
    ~ArchiveItem() override = default;

    bool IsArchive() const override
    {
      return true;
    }

    const cs::csFileSystemArchive *m_fsArchive;
  };

  struct PathItem : public Item
  {
    ~PathItem() override = default;

    bool IsPath() const override
    {
      return true;
    }
  };

private:

  void LoadArchive(const cs::iArchive *archive);
  void LoadPath(Item *parent, QDir dir, QString path);

  std::vector<ArchiveItem *> m_archives;

  int IndexOf(Item *parent, Item *child) const;

  int SelfIndex(Item *child) const;
};
