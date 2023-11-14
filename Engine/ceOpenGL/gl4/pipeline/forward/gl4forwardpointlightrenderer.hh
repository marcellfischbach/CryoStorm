

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/openglconstants.hh>
#include <ceOpenGL/gl4/pipeline/pointsm/gl4pointsmfilter.hh>
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
  void RenderShadowBuffer(GL4PointLight *pointLight, const Camera &camera, const Projector &projector);
  void RenderShadowMap(GL4PointLight *pointLight, const Camera &camera, const Projector &projector);
  void FilterShadowMap(size_t lightIdx);
  void ApplyShadowMapToDevice(GL4PointLight *pointLight, size_t lightIdx);


  GL4TextureCube *GetPointLightShadowBufferColor();
  GL4TextureCube *GetPointLightShadowBufferDepth();
  iSampler *GetShadowBufferColorSampler();
  iSampler *GetShadowBufferDepthSampler();


  GL4RenderTarget2D *CreatePointLightShadowMap();
  GL4RenderTarget2D *GetPointLightShadowMapTemp();
  GL4RenderTarget2D *GetPointLightShadowMap(Size idx);
  iSampler *GetShadowMapColorSampler();

private:
  GL4Device *m_device = nullptr;
  iGfxScene *m_scene = nullptr;
  iTexture2D *m_depthBuffer = nullptr;

  std::vector<GL4PointLight *> m_shadowPointLights;


  size_t m_pointLightShadowBufferSize;

  //
  // Shadow Buffer

  GL4TextureCube *m_shadowBufferColor = nullptr;
  GL4TextureCube *m_shadowBufferDepth = nullptr;
  iSampler *m_shadowBufferColorSampler = nullptr;
  iSampler *m_shadowBufferDepthSampler = nullptr;

  iShader *m_shadowMappingShader = nullptr;
  iShaderAttribute *m_attrLightPosition = nullptr;
  iShaderAttribute *m_attrMappingBias = nullptr;
  iShaderAttribute *m_attrShadowBuffer = nullptr;
  iShaderAttribute *m_attrDepthBuffer = nullptr;


  //
  // Shadow Map

  iSampler *m_shadowMapColorSampler = nullptr;

  GL4RenderTarget2D* m_pointLightShadowMapTemp = nullptr;
  std::array<GL4RenderTarget2D *, MaxLights> m_pointLightShadowMap;

  size_t m_shadowMapWidth;
  size_t m_shadowMapHeight;


  enum class ShadowSamplingMode
  {
    Plain,
    PCF,
    VSM
  };
  ShadowSamplingMode m_shadowSamplingFilter;

  unsigned         m_shadowFBO = 0;
  GL4PointSMFilter m_shadowMapFilter;

};


}