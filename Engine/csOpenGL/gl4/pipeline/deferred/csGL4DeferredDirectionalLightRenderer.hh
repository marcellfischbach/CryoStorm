#pragma once

#include <csOpenGL/gl4/pipeline/pssm/csGL4PSSMRenderer.hh>


namespace cs
{

class csCamera;
class csProjector;
class csGBuffer;
struct iDevice;
struct iDirectionalLight;
struct iGfxScene;
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;

namespace opengl
{

class csGL4DirectionalLight;
class csGL4RenderTarget2D;

class csGL4DeferredDirectionalLightRenderer
{
public:
  csGL4DeferredDirectionalLightRenderer() = default;
  ~csGL4DeferredDirectionalLightRenderer() = default;

  bool Initialize();

  void SetDevice(iDevice *device);
  void SetScene(iGfxScene *scene);
  void ResetShadowBuffer();

  void Render(const csCamera *camera,
              const csProjector *projector,
              const csGBuffer *gBuffer,
              csGL4DirectionalLight *light,
              iRenderTarget2D *target);


private:
  csGL4RenderTarget2D *GetShadowMap();
  csGL4PSSMShadowBufferObject &GetShadowBuffer();


  csRef<csGL4Device> m_device;
  csRef<iGfxScene> m_scene;

  struct LightRenderShader
  {
    csAssetRef<iShader> m_shader;
    iShaderAttribute    *m_attrDiffuseRoughness       = nullptr;
    iShaderAttribute    *m_attrNormal                 = nullptr;
    iShaderAttribute    *m_attrDepth                  = nullptr;
    iShaderAttribute    *m_attrShadowMap              = nullptr;
    iShaderAttribute    *m_attrLightColor             = nullptr;
    iShaderAttribute    *m_attrLightAmbientColor      = nullptr;
    iShaderAttribute    *m_attrLightNegLightDirection = nullptr;
    iShaderAttribute    *m_attrCameraPosition         = nullptr;
  };

  LightRenderShader m_nonShadow;
  LightRenderShader m_shadow;

  csGL4PSSMRenderer m_pssmRenderer;

  csRef<csGL4RenderTarget2D>                 m_shadowMap  = nullptr;
  csGL4PSSMShadowBufferObject                m_shadowBuffer;
  size_t                                     m_lightIdx   = 0;
  std::array<csRef<csGL4RenderTarget2D>, 4>  m_shadowMaps = {nullptr, nullptr, nullptr, nullptr};
  std::array<csGL4PSSMShadowBufferObject, 4> m_shadowBuffers;

};

}
}



