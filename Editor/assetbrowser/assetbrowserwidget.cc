//
// Created by Marcell on 16.09.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AssetBrowserWidget.h" resolved

#include <assetbrowser/assetbrowserwidget.hh>
#include <assetbrowser/assetbrowserfoldermodel.hh>
#include <assetbrowser/assetbrowsertreemodel.hh>
#include "ui_AssetBrowserWidget.h"


AssetBrowserWidget::AssetBrowserWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AssetBrowserWidget)
{
  ui->setupUi(this);

  m_treeModel  = new AssetBrowserTreeModel(this);
  m_folderModel = new AssetBrowserFolderModel(this);
  m_treeModel->Reload();

  ui->treeView->setModel(m_treeModel);
  ui->listView->setModel(m_folderModel);
}

AssetBrowserWidget::~AssetBrowserWidget()
{
  delete ui;
  m_folderModel = nullptr;
  m_treeModel = nullptr;
}


void AssetBrowserWidget::on_treeView_activated(const QModelIndex &index)
{
  const std::string &path = m_treeModel->GetPath(index);
  m_folderModel->SetFolder(path);
}

void AssetBrowserWidget::on_treeView_clicked(const QModelIndex &index)
{
  const std::string &path = m_treeModel->GetPath(index);
  m_folderModel->SetFolder(path);
}
