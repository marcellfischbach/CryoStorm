//
// Created by Marcell on 08.12.2024.
//

#include <editors/shadergraph/ShaderGraphEditorWidget.hh>
#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <editors/shadergraph/ShaderGraphNodePalletTreeModel.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <csCore/entity/csCameraState.hh>
#include <csCore/entity/csEntity.hh>
#include <csCore/entity/csWorld.hh>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include "ui_ShaderGraphEditorWidget.h"

using namespace cs;

ShaderGraphEditorWidget::ShaderGraphEditorWidget(QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint)
    , m_gui(new Ui::ShaderGraphEditorWidget)
    , m_shaderGraph(new csShaderGraph())
    , m_palletModel(new ShaderGraphNodePalletTreeModel())
{
  m_gui->setupUi(this);
  m_gui->pallet->setModel(m_palletModel);
  connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));



//  m_gui->graph->setScene(m_scene);
  m_gui->graph->setRenderHints(QPainter::Antialiasing);
  m_gui->graph->SetShaderGraph(m_shaderGraph);




//  InsertNode(m_shaderGraph, QPointF(0.0, 0.0));


  QList<int> sizes;
  sizes << 250
        << width() - 200
        << 250;
  m_gui->mainSplitter->setSizes(sizes);

}


ShaderGraphEditorWidget::~ShaderGraphEditorWidget()
{
  CS_RELEASE(m_shaderGraph);
  delete m_gui;
}







void ShaderGraphEditorWidget::on_graph_initialize(cs::csWorld *world)
{
  CS_SET(m_world, world);


  m_camera = new csCameraState();
  m_camera->SetClearColor(csColor4f(0.0f, 0.0f, 0.0f, 1.0f));
  m_camera->SetClearMode(eClearMode::DepthColor);
  m_camera->SetClearColorMode(eClearColorMode::PlainColor);

  m_cameraEntity = new csEntity("Camera");
  m_cameraEntity->AttachState(m_camera);

  m_world->Attach(m_cameraEntity);

}