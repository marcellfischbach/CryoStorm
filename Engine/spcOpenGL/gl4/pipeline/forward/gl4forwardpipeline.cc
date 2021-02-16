
#include <spcOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>
#include <spcOpenGL/gl4/gl4device.hh>
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/projector.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/math/clipper/boxclipper.hh>


namespace spc::opengl
{

const Size MaxLights = 4;

GL4ForwardPipeline::GL4ForwardPipeline()
{

}

GL4ForwardPipeline::~GL4ForwardPipeline() noexcept
{

}
void GL4ForwardPipeline::Render(Camera& camera, Projector& projector, iDevice* device, GfxScene* scene)
{
  camera.Bind(device);
  projector.Bind(device);
  BoxClipper clipper(Vector3f(-1000.0f, -1000.0f, -1000.0f), Vector3f(1000.0f, 1000.0f, 1000.0f));

  std::vector<GfxMesh*> m_transparency;


  // get all global lights from the scene
  const GfxLight* lights[MaxLights];
  Size offset = 0;
  scene->ScanLights(&clipper, GfxScene::eSM_Global, [&lights, &offset](GfxLight* light) {
    if (offset >= MaxLights)
    {
      return false;
    }
    lights[offset++] = light;
    return true;
  });





  printf ("Offset: %zu\n", offset);

  scene->Render(device, spc::eRP_Forward);

}

}