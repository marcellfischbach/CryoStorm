  //
// Created by mcel on 17.09.2024.
//

#pragma once

#include <QAbstractItemModel>


class AssetBrowserFolderModel : public QAbstractItemModel
{

public:
  AssetBrowserFolderModel(QObject* parent);
  void SetFolder (const std::string &folder);


public:
  QModelIndex index(int row, int column, const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;

private:

  void LoadFolder ();


  struct Item
  {
    std::string Name;
  };

  std::string m_folder;
  std::vector<Item> m_items;
};
