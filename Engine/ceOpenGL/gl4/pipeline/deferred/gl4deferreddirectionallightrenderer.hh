#pragma once

namespace ce
{

class Camera;
class Projector;
class GBuffer;
struct iDevice;
struct iDirectionalLight;
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;

namespace opengl
{


class GL4DeferredDirectionalLightRenderer
{
public:
  GL4DeferredDirectionalLightRenderer();

  bool Initialize();
  void Render(iDevice *device,
              const Camera *camera,
              const Projector *projector,
              const GBuffer *gBuffer,
              const iDirectionalLight *light,
              iRenderTarget2D *target);

private:
  iShader          *m_shader                     = nullptr;
  iShaderAttribute *m_attrDiffuseRoughness       = nullptr;
  iShaderAttribute *m_attrNormal                 = nullptr;
  iShaderAttribute *m_attrDepth                  = nullptr;
  iShaderAttribute *m_attrLightColor             = nullptr;
  iShaderAttribute *m_attrLightAmbientColor      = nullptr;
  iShaderAttribute *m_attrLightNegLightDirection = nullptr;
  iShaderAttribute *m_attrCameraPosition         = nullptr;
};

}
}



