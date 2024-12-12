//
// Created by MCEL on 22.02.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.hh"
#include "ui_MainWindow.h"
#include <assetbrowser/assetbrowserdockwidget.hh>
#include <sceneview/SceneViewWidget.hh>
#include <editors/shadergraph/ShaderGraphEditorWidget.hh>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  m_assetBrowser = new AssetBrowserDockWidget(this);
  m_sceneViewWidget = new SceneViewWidget(this);

  addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_assetBrowser);

  setCentralWidget(m_sceneViewWidget);


  QTimer::singleShot(250, [this] (){ on_actionCreateShaderGraph_triggered(false);});
}

void MainWindow::showEvent(QShowEvent *event)
{
  printf ("Show\n");
}
void MainWindow::on_actionCreateShaderGraph_triggered(bool checked)
{
  ShaderGraphEditorWidget *dlg = new ShaderGraphEditorWidget (this);
  dlg->show();
}

MainWindow::~MainWindow()
{
  delete ui;
}
