
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
        : m_frame(0)
{

}

GL4ForwardPipeline::~GL4ForwardPipeline() noexcept
{

}
void GL4ForwardPipeline::Render(Camera& camera, Projector& projector, iDevice* device, GfxScene* scene)
{
  ++m_frame;


  camera.Bind(device);
  projector.Bind(device);
  BoxClipper clipper(Vector3f(-1000.0f, -1000.0f, -1000.0f), Vector3f(1000.0f, 1000.0f, 1000.0f));

  std::vector<GfxMesh*> m_transparency;


  // get all global finalRenderLights from the scene
  const GfxLight* finalRenderLights[MaxLights];
  Size finalRenderLightOffset = 0;
  scene->ScanLights(&clipper, GfxScene::eSM_Global, [&finalRenderLights, &finalRenderLightOffset](GfxLight* light) {
    if (finalRenderLightOffset >= MaxLights)
    {
      return false;
    }
    finalRenderLights[finalRenderLightOffset++] = light;
    return true;
  });


  m_dynamicLights.clear();
  m_staticLights.clear();
  m_staticLightsNew.clear();
  scene->ScanLights(&clipper,
                    GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
                    [this](GfxLight* light) {
                      if (light->IsStatic())
                      {
                        m_staticLights.push_back(light);
                        if (light->GetFrame() == 0)
                        {
                          m_staticLightsNew.push_back(light);
                        }
                      }
                      else
                      {
                        m_dynamicLights.push_back(light);
                      }

                      return true;
                    });


  printf("Offset: %zu\n", finalRenderLightOffset);

  scene->Render(device, spc::eRP_Forward);

}

}