#include <ceOpenGL/gl4/pipeline/deferred/gl4deferredpipeline.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/gbuffer.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
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


void GL4DeferredPipeline::Render(iRenderTarget2D* target, const GfxCamera* camera, iDevice* device, iGfxScene* scene)
{
  m_gBuffer->Update(device, target->GetWidth(), target->GetHeight());
}

void GL4DeferredPipeline::SetupVariables(iRenderTarget2D* target,
                                        const GfxCamera* camera,
                                        iDevice* device,
                                        iGfxScene* scene)
{
//  m_device    = device;
//  m_gfxCamera = camera;
//  m_camera    = camera->GetCamera();
//  m_projector = camera->GetProjector();
  m_scene     = scene;
//  m_target    = target;

//  m_pointLightRenderer.SetDevice(device);
//  m_pointLightRenderer.SetScene(scene);
//  m_pointLightRenderer.Clear();
//  m_directionalLightRenderer.SetDevice(device);
//  m_directionalLightRenderer.SetScene(scene);
//  m_directionalLightRenderer.Clear();

}

bool transparent_mesh_compare_less(const GfxMesh* mesh0, const GfxMesh* mesh1)
{
  return false;
}

void GL4DeferredPipeline::ScanVisibleMeshes(iClipper* clipper)
{
  m_collector.Clear();
  m_scene->ScanMeshes(clipper, m_collector);

  std::vector<GfxMesh*>& defaultMeshes     = m_collector.GetMeshes(eRenderQueue::Default);
  std::vector<GfxMesh*>& transparentMeshes = m_collector.GetMeshes(eRenderQueue::Transparency);

  std::sort(defaultMeshes.begin(), defaultMeshes.end(), material_shader_compare_less_forward);
  std::sort(transparentMeshes.begin(), transparentMeshes.end(), transparent_mesh_compare_less);
}



}



