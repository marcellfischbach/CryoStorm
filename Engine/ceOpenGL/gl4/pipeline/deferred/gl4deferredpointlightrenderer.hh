

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/gl4/pipeline/pointsm/gl4pointsmrenderer.hh>

namespace cryo
{

class csCamera;
class csMatrix4f;
class csProjector;
class csGBuffer;
class csSettings;

struct iDevice;
struct iGfxScene;
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;

namespace opengl
{

class GL4Device;
class GL4PointLight;
class GL4RenderTarget2D;

class GL4DeferredPointLightRenderer
{
public:
  GL4DeferredPointLightRenderer() = default;
  ~GL4DeferredPointLightRenderer() = default;

  bool Initialize();

  void SetDevice(iDevice *device);
  void SetScene(iGfxScene *scene);
  void ResetShadowMaps ();

  void Render(const csCamera *camera,
              const csProjector *projector,
              const csGBuffer *gBuffer,
              const GL4PointLight *light,
              iRenderTarget2D *target);

  GL4PointSMRenderer &GetShadowRenderer()
  {
    return m_shadowRenderer;
  }

private:
  GL4RenderTarget2D *GetShadowMap();
  void CalcSphereSizeOnScreen(const csMatrix4f &camera,
                              const csMatrix4f &projection,
                              const GL4PointLight *light,
                              csVector2f &outBottomLeft,
                              csVector2f &outTopRight) const;
  csVector2f OnScreen(const csVector3f &v, const csMatrix4f &m) const;


  GL4Device *m_device;
  iGfxScene *m_scene;

  struct LightRenderShader
  {
    iShader          *m_shader                = nullptr;
    iShaderAttribute *m_attrRectMin           = nullptr;
    iShaderAttribute *m_attrRectMax           = nullptr;
    iShaderAttribute *m_attrDiffuseRoughness  = nullptr;
    iShaderAttribute *m_attrNormal            = nullptr;
    iShaderAttribute *m_attrDepth             = nullptr;
    iShaderAttribute *m_attrShadowMap         = nullptr;
    iShaderAttribute *m_attrLightColor        = nullptr;
    iShaderAttribute *m_attrLightAmbientColor = nullptr;
    iShaderAttribute *m_attrLightPosition     = nullptr;
    iShaderAttribute *m_attrLightRange        = nullptr;
    iShaderAttribute *m_attrCameraPosition    = nullptr;
  };

  LightRenderShader m_nonShadow;
  LightRenderShader m_shadow;


  GL4RenderTarget2D  *m_shadowMap = nullptr;
  GL4PointSMRenderer m_shadowRenderer;

};


}

}