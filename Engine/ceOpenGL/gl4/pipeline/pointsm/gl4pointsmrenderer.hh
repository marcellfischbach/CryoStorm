//
// Created by Marcell on 11.11.2023.
//

#pragma once

#include <ceOpenGL/gl4/pipeline/pointsm/gl4pointsmfilter.hh>
#include <vector>

namespace ce
{

class Camera;
class GfxMesh;
class Projector;
class Settings;

struct iGfxScene;
struct iSampler;
struct iShader;
struct iShaderAttribute;
struct iTexture2D;

namespace opengl
{

class GL4Device;
class GL4PointLight;
class GL4RenderTarget2D;
class GL4RenderTargetCube;

class GL4PointSMRenderer
{

public:
  GL4PointSMRenderer() = default;
  ~GL4PointSMRenderer() = default;

  void Initialize(Settings &settings);

  void SetDepthBuffer(iTexture2D *depthBuffer);
  void SetDevice(GL4Device *device);
  void SetScene(iGfxScene *scene);
  GL4RenderTarget2D *CreateShadowMap();
  void SetShadowMap(GL4RenderTarget2D *shadowMap);
  GL4RenderTarget2D *GetShadowMap();
  void RenderShadow(const GL4PointLight *pointLight, const Camera &camera, const Projector &projector);

  bool IsShadowMapValid(GL4RenderTarget2D *shadowMap) const;

private:
  void RenderShadowBuffer(const GL4PointLight *pointLight, const Camera &camera, const Projector &projector);
  void RenderShadowMap(const GL4PointLight *pointLight, const Camera &camera, const Projector &projector);
  void FilterShadowMap();

  GL4RenderTargetCube *GetShadowBuffer ();
  GL4RenderTarget2D *GetShadowMapTemp();

  iSampler *GetShadowMapColorSampler();
  iSampler *GetShadowBufferColorSampler();
  iSampler *GetShadowBufferDepthSampler();
private:
  GL4Device *m_device = nullptr;
  iGfxScene *m_scene  = nullptr;


  iTexture2D *m_depthBuffer = nullptr;

  GL4RenderTargetCube *m_pointLightShadowBuffer    = nullptr;
  size_t              m_pointLightShadowBufferSize = 0;


  GL4RenderTarget2D *m_pointLightShadowMapTemp  = nullptr;
  GL4RenderTarget2D *m_pointLightShadowMap      = nullptr;
  size_t            m_pointLightShadowMapWidth  = 0;
  size_t            m_pointLightShadowMapHeight = 0;

  enum class ShadowSamplingMode
  {
    Plain,
    PCF,
    VSM
  };

  ShadowSamplingMode m_shadowSamplingMode;
  iSampler           *m_shadowMapColorSampler    = nullptr;
  iSampler           *m_shadowBufferColorSampler = nullptr;
  iSampler           *m_shadowMapDepthSampler    = nullptr;

  iShader          *m_shadowMappingShader = nullptr;
  iShaderAttribute *m_attrLightPosition   = nullptr;
  iShaderAttribute *m_attrMappingBias     = nullptr;
  iShaderAttribute *m_attrShadowBuffer    = nullptr;
  iShaderAttribute *m_attrDepthBuffer     = nullptr;


  GL4PointSMFilter m_shadowMapFilter;

  std::vector<GfxMesh *> m_meshesCache;
};

} // ce
} // opengl
