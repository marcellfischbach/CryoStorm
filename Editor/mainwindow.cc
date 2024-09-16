//
// Created by MCEL on 22.02.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.hh"
#include "ui_MainWindow.h"
#include <assetbrowser/assetbrowserwidget.hh>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  auto assetBrowser = new AssetBrowserWidget();
  setCentralWidget(assetBrowser);
}

MainWindow::~MainWindow()
{
  delete ui;
}
