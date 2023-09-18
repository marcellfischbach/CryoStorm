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


namespace ce::opengl
{


GL4DeferredPipeline::GL4DeferredPipeline()
    : iRenderPipeline()
    , m_gBuffer(new GBuffer())
{
  CE_CLASS_GEN_CONSTR;
}

GL4DeferredPipeline::~GL4DeferredPipeline()
{

}


void GL4DeferredPipeline::Initialize()
{

}


void GL4DeferredPipeline::Render(iRenderTarget2D *target, const GfxCamera *camera, iDevice *device, iGfxScene *scene)
{
  SetupVariables(target, camera, device, scene);
  RenderGBuffer(device, camera, scene, target->GetWidth(), target->GetHeight());


  device->SetRenderTarget(target);
  device->SetRenderBuffer(0);
  device->SetDepthTest(false);
  device->SetDepthWrite(false);
  device->SetColorWrite(true, true, true, true);
//  device->Clear(true, Color4f(0.5f, 0.0f, 0.0f, 0.0f), false, 1.0f, true, 0);
  device->RenderFullscreen(m_gBuffer->getNormal());
}

void GL4DeferredPipeline::RenderGBuffer(iDevice *device,
                                        const GfxCamera *camera,
                                        iGfxScene *scene,
                                        uint16_t width,
                                        uint16_t height)
{
  m_gBuffer->Update(device, width, height);
  CameraClipper clppr(*m_camera, *m_projector);
  ScanVisibleMeshes(&clppr);
  BindCamera();

  device->SetRenderTarget(m_gBuffer->getGBuffer());
  device->SetRenderBuffer(m_gBuffer->GetBufferIDs());
  device->SetDepthTest(true);
  device->SetDepthWrite(true);
  device->SetColorWrite(true, true, true, true);
  device->Clear(true, Color4f(0.0f, 0.0f, 0.0f, 0.0f), true, 1.0f, true, 0);
  std::vector<GfxMesh *> &meshes = m_collector.GetMeshes(eRenderQueue::Default);
  for (const auto mesh: meshes)
  {
    mesh->Render(device, eRP_GBuffer);
  }

}

void GL4DeferredPipeline::SetupVariables(iRenderTarget2D *target,
                                         const GfxCamera *camera,
                                         iDevice *device,
                                         iGfxScene *scene)
{
  m_device    = device;
  m_gfxCamera = camera;
  m_camera    = camera->GetCamera();
  m_projector = camera->GetProjector();
  m_scene = scene;
//  m_target    = target;

//  m_pointLightRenderer.SetDevice(device);
//  m_pointLightRenderer.SetScene(scene);
//  m_pointLightRenderer.Clear();
//  m_directionalLightRenderer.SetDevice(device);
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



