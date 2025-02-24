//
// Created by Marcell on 08.12.2024.
//

#include <csEditor/editors/shadergraph/ShaderGraphEditorWidget.hh>
#include <csEditor/editors/shadergraph/ShaderGraphNodeItem.hh>
#include <csEditor/editors/shadergraph/ShaderGraphNodePalletTreeModel.hh>
#include <csEditor/editors/shadergraph/ShaderGraphSaver.hh>
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

#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include "ui_ShaderGraphEditorWidget.h"


using namespace cs;

ShaderGraphEditorWidget::ShaderGraphEditorWidget(csShaderGraph *shaderGraph,
                                                 const csAssetLocator &locator,
                                                 QWidget *parent)
    : EditorWidget(parent)
    , m_gui(new Ui::ShaderGraphEditorWidget)
    , m_shaderGraph(shaderGraph)
    , m_locator(locator)
    , m_palletModel(new ShaderGraphNodePalletTreeModel())
{
  m_gui->setupUi(this);

  m_gui->propertiesStack->setCurrentWidget(m_gui->noSelection);
//  m_gui->shaderGraphPage->SetShaderGraph(m_shaderGraph);

  m_gui->pallet->setModel(m_palletModel);
  connect(m_gui->btnSave, &QPushButton::clicked, this, &ShaderGraphEditorWidget::onBtnSaveClicked);
  connect(m_gui->graph,
          &ShaderGraphGraphicsView::SelectionChanged,
          this,
          &ShaderGraphEditorWidget::onNodeSelectionChanged);

  connect(m_gui->nodePage,
          &ShaderGraphEditorNodePropertiesWidget::ResourceNameChanged,
          m_gui->graph,
          &ShaderGraphGraphicsView::UpdateSelectedNodes);
  connect(m_gui->preview,
          &SceneViewWidget::initialize,
          this,
          &ShaderGraphEditorWidget::onPreviewInitialized);
  connect(m_gui->preview, &SceneViewWidget::mousePressed,
          this, &ShaderGraphEditorWidget::onPreviewMousePressed);
  connect(m_gui->preview, &SceneViewWidget::mouseReleased,
          this, &ShaderGraphEditorWidget::onPreviewMouseReleased);
  connect(m_gui->preview, &SceneViewWidget::mouseMoved,
          this, &ShaderGraphEditorWidget::onPreviewMouseMoved);
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


void ShaderGraphEditorWidget::onPreviewInitialized(cs::csWorld *world)
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
  m_light->SetShadowMapBias(0.03);
  m_lightEntity = new csEntity("Light");
  m_lightEntity->AttachState(m_light);

  m_light->GetTransform()
         .SetRotation(csQuaternion::FromAxisAngle(cs::csVector3f(1.0f, 0.5f, 0.5f).Normalized(),
                                                  cs::ceDeg2Rad(45.0f)))
         .Finish();
  m_world->Attach(m_lightEntity);


  auto cubeRenderMesh = csCubeMeshGenerator().Generate();
  auto cubeMesh       = new csMesh();
  cubeMesh->AddMaterialSlot("Slot0", nullptr);
  cubeMesh->AddSubMesh(cubeRenderMesh.Data(), 0);
  m_cube = new csStaticMeshState();
  m_cube->SetMesh(cubeMesh);
  m_cube->SetMaterial(0, m_shaderGraph);
  m_cubeEntity = new csEntity();
  m_cubeEntity->AttachState(m_cube);

  m_world->Attach(m_cubeEntity);
}

void ShaderGraphEditorWidget::onPreviewMousePressed(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    if (m_rightButtonArmed)
    {
      return;
    }
    m_leftButtonArmed = true;
    m_buttonPos   = event->pos();
  }
  if (event->button() == Qt::RightButton)
  {
    if (m_leftButtonArmed)
    {
      return;
    }
    m_rightButtonArmed = true;
    m_buttonPos = event->pos();
  }
}

void ShaderGraphEditorWidget::onPreviewMouseReleased(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    if (m_leftButtonArmed)
    {
      m_leftButtonArmed = false;
      m_cameraRotationV = RotationVFrom(m_cameraRotationV, event->pos(), false);
      m_cameraRotationH = RotationHFrom(m_cameraRotationH, event->pos(), false);
      UpdateCamera(m_cameraRotationH, m_cameraRotationV);
    }
  }
  if (event->button() == Qt::RightButton)
  {
    if (m_rightButtonArmed)
    {
      m_rightButtonArmed = false;
      m_lightRotationV = RotationVFrom(m_lightRotationV, event->pos(), true);
      m_lightRotationH = RotationHFrom(m_lightRotationH, event->pos(), true);
      UpdateLight(m_lightRotationH, m_lightRotationV);
    }
  }
}

void ShaderGraphEditorWidget::onPreviewMouseMoved(QMouseEvent *event)
{
  if (m_leftButtonArmed)
  {
    double rotationV = RotationVFrom(m_cameraRotationV, event->pos(), false);
    double rotationH = RotationHFrom(m_cameraRotationH, event->pos(), false);
    UpdateCamera(rotationH, rotationV);
  }
  if (m_rightButtonArmed)
  {
    double rotationV = RotationVFrom(m_lightRotationV, event->pos(), true);
    double rotationH = RotationHFrom(m_lightRotationH, event->pos(), true);
    UpdateLight(rotationH, rotationV);
  }
}

double ShaderGraphEditorWidget::RotationHFrom(double baseRotation, const QPoint &point, bool invert)
{
  QPoint delta     = point - m_buttonPos;
  if (invert)
  {
    delta *= -1;
  }
  double rotationH = baseRotation - (delta.y() * 0.01);
  rotationH = std::max(rotationH, -M_PI_2 + 0.1);
  rotationH = std::min(rotationH, M_PI_2 - 0.1);

  return rotationH;
}


double ShaderGraphEditorWidget::RotationVFrom(double baseRotation, const QPoint &point, bool invert)
{
  QPoint delta     = point - m_buttonPos;
  if (invert)
  {
    delta *= -1;
  }
  double rotationV = baseRotation + (delta.x() * 0.01);
  rotationV = std::fmod(rotationV, M_PI * 2.0);

  return rotationV;
}

void ShaderGraphEditorWidget::UpdateCamera(double rotationH, double rotationV)
{
  csMatrix4f matV     = csMatrix4f::Rotation(csVector3f(0.0f, 1.0f, 0.0f), rotationV);
  csMatrix4f matH     = csMatrix4f::Rotation(csVector3f(1.0f, 0.0f, 0.0f), rotationH);
  csMatrix4f rotation = matV * matH;

  csVector3f eye = rotation * csVector3f(0.0f, 0.0f, 3.0f);
  m_camera->GetTransform()
          .SetTranslation(eye)
          .LookAt(csVector3f(0.0f, 0.0f, 0.0f), csVector3f(0.0f, 1.0f, 0.0f))
          .Finish();
}

void ShaderGraphEditorWidget::UpdateLight(double rotationH, double rotationV)
{
  csMatrix4f matV     = csMatrix4f::Rotation(csVector3f(0.0f, 1.0f, 0.0f), rotationV);
  csMatrix4f matH     = csMatrix4f::Rotation(csVector3f(1.0f, 0.0f, 0.0f), rotationH);
  csMatrix4f rotation = matV * matH;

  csVector3f eye = rotation * csVector3f(0.0f, 0.0f, 3.0f);
  m_light->GetTransform()
          .SetTranslation(eye)
          .LookAt(csVector3f(0.0f, 0.0f, 0.0f), csVector3f(0.0f, 1.0f, 0.0f))
          .Finish();
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

void ShaderGraphEditorWidget::onResourceNameChanged()
{
  m_gui->graph->UpdateSelectedNodes();
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


  iShaderGraphCompiler::Parameters params {};
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
