//
// Created by Marcell on 16.09.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AssetBrowserWidget.h" resolved

#include <assetbrowser/assetbrowserwidget.hh>
#include <assetbrowser/assetbrowsertreemodel.hh>
#include "ui_AssetBrowserWidget.h"


AssetBrowserWidget::AssetBrowserWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::AssetBrowserWidget)
{
  ui->setupUi(this);

  auto tableModel = new AssetBrowserTreeModel();
  tableModel->Reload();

  ui->treeView->setModel(tableModel);
}

AssetBrowserWidget::~AssetBrowserWidget()
{
  delete ui;
}
