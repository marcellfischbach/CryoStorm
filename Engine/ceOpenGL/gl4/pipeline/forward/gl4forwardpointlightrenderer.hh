

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/openglconstants.hh>
#include <ceOpenGL/gl4/pipeline/pointsm/gl4pointsmrenderer.hh>
#include <ceCore/types.hh>
#include <map>
#include <vector>
#include <array>

namespace ce
{

struct iDevice;
struct iSampler;
struct iShader;
struct iShaderAttribute;
struct iGfxScene;
struct iTexture2D;
class Settings;

class Camera;
class Projector;

}

namespace ce::opengl
{

class GL4Device;
class GL4PointLight;
class GL4TextureCube;
class GL4RenderTarget2D;
class GL4RenderTargetCube;

class GL4ForwardPointLightRenderer
{
public:
  GL4ForwardPointLightRenderer();
  ~GL4ForwardPointLightRenderer();

  void Initialize(Settings &settings);

  void SetDevice(iDevice *device);
  void SetScene(iGfxScene *scene);
  void SetDepthBuffer(iTexture2D *depthBuffer);


  void Clear();
  void Add(GL4PointLight *pointLight);

  void RenderShadowMaps(const Camera &camera,
                        const Projector &projector);

  iTexture2D* GetColorTexture();
  iTexture2D* GetDepthTexture();
private:
  void SortLights();


  void RenderShadow(GL4PointLight *pointLight, const Camera &camera, const Projector &projector, size_t lightIdx);
  void ApplyShadowMapToDevice(GL4PointLight *pointLight, size_t lightIdx);
  GL4RenderTarget2D *GetPointLightShadowMap(Size idx);

private:
  GL4Device *m_device = nullptr;
  iGfxScene *m_scene = nullptr;
  iTexture2D *m_depthBuffer = nullptr;

  std::vector<GL4PointLight *> m_shadowPointLights;
  std::array<GL4RenderTarget2D *, MaxLights> m_pointLightShadowMap;

  GL4PointSMRenderer m_shadowRenderer;



};


}