//
// Created by Marcell on 12.12.2024.
//

#pragma once

#include <QAbstractItemModel>
#include <string>
#include <vector>


namespace cs
{
class csClass;
}
class ShaderGraphNodePalletTreeModel : public QAbstractItemModel
{
public:

  static const std::string s_mime;

  ShaderGraphNodePalletTreeModel();

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  Qt::DropActions supportedDragActions() const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QStringList mimeTypes() const override;
  QMimeData *mimeData(const QModelIndexList &indexes) const override;

private:
  struct NodeDecl
  {
    const cs::csClass* Cls;
    QString Name;
  };

  std::vector<NodeDecl> m_nodes;
};
