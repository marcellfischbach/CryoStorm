#pragma once

#include <QAbstractTableModel>
#include <csCore/resource/csVFS.hh>
#include <QDir>


class AssetBrowserTreeModel : public QAbstractItemModel
{
public:
  AssetBrowserTreeModel(QObject *parent);

  void Reload();

  std::string GetPath (const QModelIndex &index) const;

public:

  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;


private:

  struct PathItem;
  struct Item
  {
    Item* Parent;
    std::vector<PathItem*> Children;
    std::string Name;
    std::string FullPath;

    virtual ~Item()
    {
      
      for (auto child : Children)
      {
        delete child;
      }
      Children.clear();
    }


  };

  struct ArchiveItem : public Item
  {
    ~ArchiveItem() override = default;
  };

  struct PathItem : public Item
  {

    ~PathItem() override = default;
  };

  void LoadArchive (const cryo::iArchive *archive);
  void LoadPath(Item* parent, QDir dir, QString path);

  std::vector<ArchiveItem*> m_archives;

  int IndexOf(Item* parent, Item* child) const;

  int SelfIndex(Item* child) const;
};
