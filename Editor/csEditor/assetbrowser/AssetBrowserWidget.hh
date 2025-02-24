//
// Created by Marcell on 16.09.2024.
//

#ifndef CRIMSONEDGE_ASSETBROWSERWIDGET_HH
#define CRIMSONEDGE_ASSETBROWSERWIDGET_HH

#include <QWidget>
#include <filesystem>

namespace std
{
namespace fs = filesystem;
}


QT_BEGIN_NAMESPACE
namespace Ui
{
class AssetBrowserWidget;
}
QT_END_NAMESPACE


namespace cs
{
class csFileSystemArchive;
}

class QMenu;
class AssetBrowserTreeModel;
class AssetBrowserFolderModel;
struct iAssetBrowserNewItem;

class AssetBrowserWidget : public QWidget
{
Q_OBJECT

public:
  explicit AssetBrowserWidget(QWidget *parent = nullptr);
  ~AssetBrowserWidget() override;

private:
  void FillNewMenu (QMenu* menu);

  void CreateNewItem (const QPoint& globalPoint, iAssetBrowserNewItem* item);

private:
  Ui::AssetBrowserWidget *ui;

  AssetBrowserTreeModel *m_treeModel;
  AssetBrowserFolderModel *m_folderModel;

  const cs::csFileSystemArchive *m_archive;
  std::fs::path m_currentPath;


private:
  void onTreeViewActivated(const QModelIndex &index);
  void onTreeViewClicked(const QModelIndex &index);
  void onFolderContentViewContextMenu (const QPoint &point);
  void onFolderContentDoubleClicked (const QModelIndex &index);
};


#endif //CRIMSONEDGE_ASSETBROWSERWIDGET_HH
