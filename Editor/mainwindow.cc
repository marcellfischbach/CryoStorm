//
// Created by MCEL on 22.02.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.hh"
#include "ui_MainWindow.h"
#include <assetbrowser/assetbrowserdockwidget.hh>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  m_assetBrowser = new AssetBrowserDockWidget(this);

  addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_assetBrowser);

}

MainWindow::~MainWindow()
{
  delete ui;
}
