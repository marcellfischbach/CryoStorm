//
// Created by Marcell on 08.12.2024.
//

#include <editors/shadergraph/ShaderGraphEditorWidget.hh>
#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <editors/shadergraph/ShaderGraphNodePalletTreeModel.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <csCore/graphics/shadergraph/iShaderGraphCompiler.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/graphics/csDefaultMeshGenerators.hh>
#include <csCore/entity/csCameraState.hh>
#include <csCore/entity/csEntity.hh>
#include <csCore/entity/csLightState.hh>
#include <csCore/entity/csStaticMeshState.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/csObjectRegistry.hh>
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
  m_gui->preview->setObjectName(QString("Shader graph preview"));
  CompileMaterial();

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


void ShaderGraphEditorWidget::on_preview_initialize(cs::csWorld *world)
{
  CS_SET(m_world, world);
  if (!m_world)
  {
    return;
  }


  m_camera = new csCameraState();
  m_camera->SetClearColor(csColor4f(0.0f, 0.0f, 0.5f, 1.0f));
  m_camera->SetClearMode(eClearMode::DepthColor);
  m_camera->SetClearColorMode(eClearColorMode::PlainColor);

  m_cameraEntity = new csEntity("Camera");
  m_cameraEntity->AttachState(m_camera);
  m_camera->GetTransform()
      .SetTranslation(-2.0f, 2.0f, -2.0f)
      .LookAt(csVector3f(0.0f, 0.0f, 0.0f), csVector3f(0.0f, 1.0f, 0.0f))
      .Finish();
  m_world->Attach(m_cameraEntity);


  iRenderMesh *cubeRenderMesh = csCubeMeshGenerator().Generate();
  auto cubeMesh = new csMesh();
  cubeMesh->AddMaterialSlot("Slot0", nullptr);
  cubeMesh->AddSubMesh(cubeRenderMesh, 0);
  m_cube = new csStaticMeshState();
  m_cube->SetMesh(cubeMesh);
  m_cube->SetMaterial(0, m_material);
  m_cubeEntity = new csEntity();
  m_cubeEntity->AttachState(m_cube);

  m_world->Attach(m_cubeEntity);
}

void ShaderGraphEditorWidget::on_btnCompile_clicked()
{
  CompileMaterial();
}


void ShaderGraphEditorWidget::CompileMaterial()
{
  auto factory = csObjectRegistry::Get<iShaderGraphCompilerFactory>();
  if (!factory)
  {
    return;
  }

  auto compiler = factory->Create();
  if (!compiler)
  {
    return;
  }
  csAutoRelease delCompiler(compiler);


  iShaderGraphCompiler::Parameters params {};
  params.DebugSources = true;
  auto material = compiler->Compile(m_shaderGraph, params);
  CS_SET(m_material, material);

  if (m_cube)
  {
    m_cube->SetMaterial(0, m_material);
  }
}

