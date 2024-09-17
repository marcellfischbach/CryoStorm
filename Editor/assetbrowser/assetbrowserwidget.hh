//
// Created by Marcell on 16.09.2024.
//

#ifndef CRIMSONEDGE_ASSETBROWSERWIDGET_HH
#define CRIMSONEDGE_ASSETBROWSERWIDGET_HH

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
class AssetBrowserWidget;
}
QT_END_NAMESPACE


class AssetBrowserTreeModel;
class AssetBrowserFolderModel;

class AssetBrowserWidget : public QWidget
{
Q_OBJECT

public:
  explicit AssetBrowserWidget(QWidget *parent = nullptr);
  ~AssetBrowserWidget() override;

private:
  Ui::AssetBrowserWidget *ui;

  AssetBrowserTreeModel *m_treeModel;
  AssetBrowserFolderModel *m_folderModel;

private slots:
  void on_treeView_activated(const QModelIndex &index);
  void on_treeView_clicked(const QModelIndex &index);
};


#endif //CRIMSONEDGE_ASSETBROWSERWIDGET_HH
