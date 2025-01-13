//
// Created by Marcell on 16.09.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AssetBrowserWidget.h" resolved

#include <assetbrowser/AssetBrowserWidget.hh>
#include <assetbrowser/AssetBrowserFolderModel.hh>
#include <assetbrowser/AssetBrowserTreeModel.hh>
#include <assetbrowser/AssetBrowserNewItemDialog.hh>
#include <assetbrowser/AssetBrowserNewItemRegistry.hh>
#include <assetbrowser/iAssetBrowserNewItem.hh>
#include <editors/EditorRegistry.hh>
#include "ui_AssetBrowserWidget.h"

#include <csCore/resource/csAssetLocator.hh>
#include <csCore/resource/csFileSystemArchive.hh>

#include <QMenu>

using namespace cs;

AssetBrowserWidget::AssetBrowserWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AssetBrowserWidget)
{
  ui->setupUi(this);

  m_treeModel = new AssetBrowserTreeModel(this);
  m_folderModel = new AssetBrowserFolderModel(this);
  m_treeModel->Reload();

  ui->treeView->setModel(m_treeModel);
  ui->folderContentView->setModel(m_folderModel);

  ui->folderContentView->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(ui->treeView, &QTreeView::activated, this, &AssetBrowserWidget::onTreeViewActivated);
  connect(ui->treeView, &QTreeView::clicked, this, &AssetBrowserWidget::onTreeViewClicked);
  connect(ui->folderContentView, &QListView::customContextMenuRequested, this,
          &AssetBrowserWidget::onFolderContentViewContextMenu);
  connect(ui->folderContentView, &QListView::doubleClicked, this, &AssetBrowserWidget::onFolderContentDoubleClicked);

}


AssetBrowserWidget::~AssetBrowserWidget()
{
  delete ui;
  m_folderModel = nullptr;
  m_treeModel = nullptr;
}


const AssetBrowserTreeModel::ArchiveItem *find_archive(const AssetBrowserTreeModel::Item *item)
{
  while (item)
  {
    if (item->IsArchive())
    {
      return dynamic_cast<const AssetBrowserTreeModel::ArchiveItem *>(item);
    }
    item = item->Parent;
  }
  return nullptr;
}

void AssetBrowserWidget::onTreeViewActivated(const QModelIndex &index)
{
  const std::string &path = m_treeModel->GetPath(index);
  m_folderModel->SetFolder(path);

  const AssetBrowserTreeModel::Item *item = m_treeModel->GetItem(index);
  const AssetBrowserTreeModel::ArchiveItem *pArchive = find_archive(item);
  m_archive = pArchive ? pArchive->m_fsArchive : nullptr;
  m_currentPath = path;
}

void AssetBrowserWidget::onTreeViewClicked(const QModelIndex &index)
{
  const std::string &path = m_treeModel->GetPath(index);
  m_folderModel->SetFolder(path);
}

void AssetBrowserWidget::onFolderContentViewContextMenu(const QPoint &point)
{
  QMenu menu;
  QMenu *newMenu = menu.addMenu("New");

  FillNewMenu(newMenu);

  QPoint global = ui->folderContentView->mapToGlobal(point);
  menu.exec(global);
}


void AssetBrowserWidget::FillNewMenu(QMenu *menu)
{
  for (auto item: AssetBrowserNewItemRegistry::Get().GetItems())
  {
    QAction *action = menu->addAction(item->GetName().c_str());

    if (item->DefaultNamingBehaviour())
    {
      connect(action, &QAction::triggered, this, [this, item, menu, action]() {
        QPoint point = menu->actionGeometry(action).center();
        QPoint global = menu->mapToGlobal(point);
        CreateNewItem(global, item);
      });
    }
    else
    {

    }
  }
}

void AssetBrowserWidget::CreateNewItem(const QPoint &globalPoint, iAssetBrowserNewItem *item)
{
  AssetBrowserNewItemDialog dlg(this);
  dlg.move(globalPoint);
  if (dlg.exec() == QDialog::Rejected)
  {
    return;
  }

  std::string name = dlg.GetName();
  if (name.empty())
  {
    return;
  }


  item->Create(m_currentPath, name);

}

void AssetBrowserWidget::onFolderContentDoubleClicked(const QModelIndex &index)
{
  const csFileSystemArchive *pArchive = m_treeModel->ExtractArchive(ui->treeView->currentIndex());
  std::string archivePath = m_treeModel->ConstructArchivePath(ui->treeView->currentIndex());
  std::string fileName = m_folderModel->GetName(index);

  cs::csAssetLocator locator(pArchive->GetName() + "@" + archivePath + fileName);
  iEditorFactory     *editorFactory = EditorRegistry::Get().GetEditor(locator);
  if (!editorFactory)
  {
    return;
  }

  editorFactory->Edit(locator, this);

}