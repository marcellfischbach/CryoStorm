

#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csOpenGL/gl4/pipeline/pointsm/csGL4PointSMRenderer.hh>

namespace cs
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

class csGL4Device;
class csGL4PointLight;
class csGL4RenderTarget2D;

class csGL4DeferredPointLightRenderer
{
public:
  csGL4DeferredPointLightRenderer() = default;
  ~csGL4DeferredPointLightRenderer() = default;

  bool Initialize();

  void SetDevice(iDevice *device);
  void SetScene(iGfxScene *scene);
  void ResetShadowMaps ();

  void Render(const csCamera *camera,
              const csProjector *projector,
              const csGBuffer *gBuffer,
              const csGL4PointLight *light,
              iRenderTarget2D *target);

  csGL4PointSMRenderer &GetShadowRenderer()
  {
    return m_shadowRenderer;
  }

private:
  csGL4RenderTarget2D *GetShadowMap();
  void CalcSphereSizeOnScreen(const csMatrix4f &camera,
                              const csMatrix4f &projection,
                              const csGL4PointLight *light,
                              csVector2f &outBottomLeft,
                              csVector2f &outTopRight) const;
  csVector2f OnScreen(const csVector3f &v, const csMatrix4f &m) const;


  csGL4Device *m_device;
  iGfxScene   *m_scene;

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


  csGL4RenderTarget2D  *m_shadowMap = nullptr;
  csGL4PointSMRenderer m_shadowRenderer;

};


}

}