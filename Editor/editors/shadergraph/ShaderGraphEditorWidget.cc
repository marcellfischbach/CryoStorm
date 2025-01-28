//
// Created by Marcell on 08.12.2024.
//

#include <editors/shadergraph/ShaderGraphEditorWidget.hh>
#include <editors/shadergraph/ShaderGraphNodeItem.hh>
#include <editors/shadergraph/ShaderGraphNodePropertiesWidget.hh>
#include <editors/shadergraph/ShaderGraphNodePalletTreeModel.hh>
#include <editors/shadergraph/ShaderGraphSaver.hh>
#include <editors/shadergraph/ShaderGraphShaderGraphPropertiesWidget.hh>
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
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/iFile.hh>
#include <csCore/csObjectRegistry.hh>

#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include "ui_ShaderGraphEditorWidget.h"


using namespace cs;

ShaderGraphEditorWidget::ShaderGraphEditorWidget(csShaderGraph *shaderGraph,
                                                 const csAssetLocator &locator,
                                                 QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint)
    , m_gui(new Ui::ShaderGraphEditorWidget)
    , m_shaderGraph(shaderGraph)
    , m_locator(locator)
    , m_palletModel(new ShaderGraphNodePalletTreeModel())
{
  m_gui->setupUi(this);
  fflush(stdout);
  fflush(stderr);

  m_gui->propertiesStack->setCurrentWidget(m_gui->noSelection);
//  m_gui->shaderGraphPage->SetShaderGraph(m_shaderGraph);

  m_gui->pallet->setModel(m_palletModel);
  connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));
  connect(m_gui->btnSave, &QPushButton::clicked, this, &ShaderGraphEditorWidget::onBtnSaveClicked);
  connect(m_gui->graph,
          &ShaderGraphGraphicsView::SelectionChanged,
          this,
          &ShaderGraphEditorWidget::onNodeSelectionChanged);

//  m_gui->graph->setScene(m_scene);
  m_gui->graph->setRenderHints(QPainter::Antialiasing);
  m_gui->graph->SetShaderGraph(m_shaderGraph);
  m_gui->nodePage->SetShaderGraph(m_shaderGraph);
  m_gui->preview->setObjectName(QString("Shader graph preview"));
  CompileMaterial();

//  InsertNode(m_shaderGraph, QPointF(0.0, 0.0));


  QList<int> sizes;
  sizes << 250
        << width() - 200
        << 250;
  m_gui->mainSplitter->setSizes(sizes);

  onNodeSelectionChanged();
}


ShaderGraphEditorWidget::~ShaderGraphEditorWidget()
{
  delete m_gui;
}


void ShaderGraphEditorWidget::on_preview_initialize(cs::csWorld *world)
{
  m_world = world;
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


  m_light = new csLightState();
  m_light->SetType(eLightType::eLT_Directional);
  m_light->SetColor(csColor4f(1.0f, 1.0f, 1.0f, 1.0f));
  m_light->SetCastShadow(true);
  m_light->SetShadowMapBias(0.003);
  m_lightEntity = new csEntity("Light");
  m_lightEntity->AttachState(m_light);

  m_light->GetTransform()
      .SetRotation(csQuaternion::FromAxisAngle(cs::csVector3f(1.0f, 0.5f, 0.5f).Normalized(),
                                               cs::ceDeg2Rad(45.0f)))
      .Finish();
  m_world->Attach(m_lightEntity);


  auto cubeRenderMesh = csCubeMeshGenerator().Generate();
  auto cubeMesh = new csMesh();
  cubeMesh->AddMaterialSlot("Slot0", nullptr);
  cubeMesh->AddSubMesh(cubeRenderMesh.Data(), 0);
  m_cube = new csStaticMeshState();
  m_cube->SetMesh(cubeMesh);
  m_cube->SetMaterial(0, m_shaderGraph);
  m_cubeEntity = new csEntity();
  m_cubeEntity->AttachState(m_cube);

  m_world->Attach(m_cubeEntity);
}

void ShaderGraphEditorWidget::on_btnCompile_clicked()
{
  CompileMaterial();
}

std::string generate_shader_graph_code(csShaderGraph *shaderGraph);

void ShaderGraphEditorWidget::onBtnSaveClicked(bool)
{
  ShaderGraphSaver(m_shaderGraph).Save(m_locator);
  CompileMaterial();
  if (csVFS::Get()->IsMasterLocator(m_locator))
  {
    m_shaderGraph->SetLocator(m_locator.AsAnonymous());
    csAssetPool::Instance().Put(m_shaderGraph);
  }
}

void ShaderGraphEditorWidget::onNodeSelectionChanged()
{
  std::vector<csSGNode *> nodes = m_gui->graph->GetSelection();

  if (nodes.empty() || nodes.size() > 1)
  {
    m_gui->propertiesStack->setCurrentWidget(m_gui->noSelection);
  }
  else
  {
    m_gui->propertiesStack->setCurrentWidget(m_gui->nodePage);
    m_gui->nodePage->SetNode(nodes[0]);
  }

//  csSGNode *node = nodes[0];
//  if (m_shaderGraph->Root() ==node)
//  {
//    m_gui->propertiesStack->setCurrentWidget(m_gui->shaderGraphPage);
//  }
//  else
//  {
//  }

}

void ShaderGraphEditorWidget::on_graph_ConnectionsChanged()
{
//  m_gui->shaderGraphPage->UpdateState();
  m_gui->nodePage->UpdateState();
}

bool ShaderGraphEditorWidget::CompileMaterial()
{
  auto factory = csObjectRegistry::Get<iShaderGraphCompilerFactory>();
  if (!factory)
  {
    return false;
  }

  auto compiler = factory->Create();
  if (!compiler)
  {
    return false;
  }


  iShaderGraphCompiler::Parameters params{};
  params.DebugSources = false;
  if (!compiler->Compile(m_shaderGraph, params))
  {
    return false;
  }

  if (m_cube)
  {
    m_cube->SetMaterial(0, m_shaderGraph);
  }
  return true;
}
