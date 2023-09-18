#include <ceOpenGL/gl4/pipeline/deferred/gl4deferredpipeline.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/math/clipper/cameraclipper.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/gbuffer.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/graphics/scene/gfxcamera.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <algorithm>
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferreddirectionallightrenderer.hh>

namespace ce::opengl
{


GL4DeferredPipeline::GL4DeferredPipeline()
    : iRenderPipeline()
    , m_gBuffer(new GBuffer())
    , m_directionalLightRenderer(new GL4DeferredDirectionalLightRenderer())
{
  CE_CLASS_GEN_CONSTR;
}

GL4DeferredPipeline::~GL4DeferredPipeline()
{

}


void GL4DeferredPipeline::Initialize()
{
  m_directionalLightRenderer->Initialize();
}


void GL4DeferredPipeline::Render(iRenderTarget2D *target, const GfxCamera *camera, iDevice *device, iGfxScene *scene)
{
  SetupVariables(target, camera, device, scene);

  CameraClipper clppr(*m_camera, *m_projector);


  RenderGBuffer(&clppr);


  m_device->SetRenderTarget(target);
  m_device->SetRenderBuffer(0);
  m_device->SetDepthTest(false);
  m_device->SetDepthWrite(false);
  m_device->SetColorWrite(true, true, true, true);
  m_device->SetBlendFactor(eBlendFactor::One, eBlendFactor::One);
  m_device->Clear(true, Color4f(0, 0, 0, 0), false, 0, false, 0);

  m_scene->ScanLights(&clppr, iGfxScene::eSM_Global | iGfxScene::eSM_Static | iGfxScene::eSM_Dynamic,
                      [this](GfxLight *light) {
                        RenderLight(light);
                        return true;
                      }
  );

  /*
  device->SetRenderTarget(target);
  device->SetRenderBuffer(0);
  device->SetDepthTest(false);
  device->SetDepthWrite(false);
  device->SetColorWrite(true, true, true, true);
//  device->Clear(true, Color4f(0.5f, 0.0f, 0.0f, 0.0f), false, 1.0f, true, 0);
  device->RenderFullscreen(m_gBuffer->GetNormal());
  */

}

void GL4DeferredPipeline::RenderGBuffer(iClipper *clipper)
{
  uint16_t width = m_target->GetWidth();
  uint16_t height = m_target->GetHeight();
  m_gBuffer->Update(m_device, width, height);
  ScanVisibleMeshes(clipper);
  BindCamera();

  m_device->SetRenderTarget(m_gBuffer->GetGBuffer());
  m_device->SetRenderBuffer(m_gBuffer->GetBufferIDs());
  m_device->SetDepthTest(true);
  m_device->SetDepthWrite(true);
  m_device->SetColorWrite(true, true, true, true);
  m_device->Clear(true, Color4f(0.0f, 0.0f, 0.0f, 0.0f), true, 1.0f, true, 0);
  std::vector<GfxMesh *> &meshes = m_collector.GetMeshes(eRenderQueue::Default);
  for (const auto mesh: meshes)
  {
    mesh->Render(m_device, eRP_GBuffer);
  }

}

void GL4DeferredPipeline::RenderLight (GfxLight *gfxLight)
{


  iLight* light = gfxLight->GetLight();
  switch (light->GetType())
  {
    case eLT_Directional:
      m_directionalLightRenderer->Render (m_target, m_gBuffer, light->Query<iDirectionalLight>());
      break;
  }
}

void GL4DeferredPipeline::SetupVariables(iRenderTarget2D *target,
                                         const GfxCamera *camera,
                                         iDevice *device,
                                         iGfxScene *scene)
{
  m_device = device;
  m_gfxCamera = camera;
  m_camera = camera->GetCamera();
  m_projector = camera->GetProjector();
  m_scene = scene;
  m_target    = target;

//  m_pointLightRenderer.SetDevice(device);
//  m_pointLightRenderer.SetScene(scene);
//  m_pointLightRenderer.Clear();
  m_directionalLightRenderer->SetDevice(device);
//  m_directionalLightRenderer.SetScene(scene);
//  m_directionalLightRenderer.Clear();

}


void GL4DeferredPipeline::BindCamera()
{
  m_camera->Bind(m_device);
  m_projector->Bind(m_device);

}


static bool transparent_mesh_compare_less(const GfxMesh *mesh0, const GfxMesh *mesh1)
{
  return false;
}

void GL4DeferredPipeline::ScanVisibleMeshes(iClipper *clipper)
{
  m_collector.Clear();
  m_scene->ScanMeshes(clipper, m_collector);
}


}



