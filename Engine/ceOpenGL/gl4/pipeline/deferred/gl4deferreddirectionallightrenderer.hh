#pragma once

#include <ceOpenGL/gl4/pipeline/pssm/gl4pssmrenderer.hh>


namespace ce
{

class Camera;
class Projector;
class GBuffer;
class Settings;
struct iDevice;
struct iDirectionalLight;
struct iGfxScene;
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;

namespace opengl
{

class GL4DirectionalLight;
class GL4RenderTarget2D;

class GL4DeferredDirectionalLightRenderer
{
public:
  GL4DeferredDirectionalLightRenderer();

  bool Initialize(Settings &settings);

  void SetDevice (iDevice* device);
  void SetScene(iGfxScene *scene);

  void Render(const Camera *camera,
              const Projector *projector,
              const GBuffer *gBuffer,
              const GL4DirectionalLight *light,
              iRenderTarget2D *target);


private:
  GL4RenderTarget2D *GetShadowMap();

  GL4Device *m_device;
  iGfxScene *m_scene;

  struct LightRenderShader
  {
    iShader          *m_shader                     = nullptr;
    iShaderAttribute *m_attrDiffuseRoughness       = nullptr;
    iShaderAttribute *m_attrNormal                 = nullptr;
    iShaderAttribute *m_attrDepth                  = nullptr;
    iShaderAttribute *m_attrShadowMap              = nullptr;
    iShaderAttribute *m_attrLightColor             = nullptr;
    iShaderAttribute *m_attrLightAmbientColor      = nullptr;
    iShaderAttribute *m_attrLightNegLightDirection = nullptr;
    iShaderAttribute *m_attrCameraPosition         = nullptr;
  };

  LightRenderShader m_nonShadow;
  LightRenderShader m_shadow;

  GL4PSSMRenderer m_pssmRenderer;

  GL4RenderTarget2D *m_shadowMap;

};

}
}



