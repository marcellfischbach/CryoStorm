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

#include <csCore/entity/csWorld.hh>
#include <csCore/entity/csEntity.hh>
#include <csCore/entity/csCameraState.hh>

using namespace cs;

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  m_assetBrowser = new AssetBrowserDockWidget(this);
  m_sceneViewWidget = new SceneViewWidget(this);

  m_sceneViewWidget->setObjectName(QString ("Main sceneview"));




  addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_assetBrowser);

  setCentralWidget(m_sceneViewWidget);

  connect (m_sceneViewWidget, SIGNAL(initialize(cs::csWorld *)), this, SLOT(initializeScene(cs::csWorld *)));

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
void MainWindow::initializeScene(cs::csWorld *world)
{

  csEntity *cameraEntity = new csEntity();
  csCameraState *camera = new csCameraState();
  camera->SetClearColor(csColor4f(0.0f, 0.0f, 0.5f, 1.0f));
  camera->SetClearColorMode(eClearColorMode::PlainColor);
  cameraEntity->AttachState(camera);
  world->Attach(cameraEntity);


}


MainWindow::~MainWindow()
{
  delete ui;
}
