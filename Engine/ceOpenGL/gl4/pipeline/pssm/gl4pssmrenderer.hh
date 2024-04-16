//
// Created by MCEL on 06.11.2023.
//

#pragma once

#include <vector>
#include <ceOpenGL/gl4/pipeline/pssm/gl4pssmfilter.hh>
#include <ceCore/graphics/scene/gfxscenecollector.hh>

namespace ce
{

struct iGfxScene;
struct iSampler;
struct iShader;
struct iShaderAttribute;
struct iTexture2D;
class Camera;
class GfxMesh;
class Projector;
class Settings;

namespace opengl
{

class GL4Device;
class GL4DirectionalLight;
class GL4RenderTarget2DArray;
class GL4RenderTarget2D;
class GL4Texture2DArray;

class GL4PSSMRenderer
{
public:
  GL4PSSMRenderer();
  ~GL4PSSMRenderer() = default;

  void Initialize(Settings &settings);

  void SetBuffers(iTexture2D *depthBuffer, iTexture2D *normalBuffer);
  void SetDevice (GL4Device *device);
  void SetScene (iGfxScene *scene);
  GL4RenderTarget2D *CreateDirectionalLightShadowMap();
  void SetShadowMap (GL4RenderTarget2D *shadowMap);
  GL4RenderTarget2D *GetShadowMap ();

  void RenderShadow(const GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);

  bool IsShadowMapValid (GL4RenderTarget2D *shadowMap) const;
private:
  void RenderShadowBuffer(const GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);
  void RenderShadowMap(const GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);
  void FilterShadowMap();


  GL4RenderTarget2D *GetDirectionalLightShadowMapTemp();
  GL4RenderTarget2D *GetDirectionalLightShadowBuffer(size_t split);
  static float GetSplitSize(const Vector3f *near, const Vector3f *far);
  iSampler *GetShadowMapColorSampler();
  iSampler *GetShadowBufferColorSampler();
  iSampler *GetShadowBufferDepthSampler();

private:
  GL4Device *m_device = nullptr;
  iGfxScene *m_scene  = nullptr;


  iTexture2D *m_depthBuffer = nullptr;
  iTexture2D *m_normalBuffer = nullptr;

  std::array<GL4RenderTarget2D*, 4> m_directionalLightShadowBuffers;
  size_t                 m_directionalLightShadowBufferSize = 0;


  GL4RenderTarget2D *m_directionalLightShadowMapTemp = nullptr;
  GL4RenderTarget2D *m_directionalLightShadowMap = nullptr;
  size_t            m_directionalLightShadowMapWidth = 0;
  size_t            m_directionalLightShadowMapHeight = 0;

  enum class ShadowSamplingMode
  {
    Plain,
    PCF,
    VSM
  };

  float m_shadowNear;
  float m_shadowFar;
  float m_splits[4];

  ShadowSamplingMode m_shadowSamplingMode;
  iSampler           *m_shadowMapColorSampler    = nullptr;
  iSampler           *m_shadowBufferColorSampler = nullptr;
  iSampler           *m_shadowMapDepthSampler    = nullptr;

  iShader          *m_shadowMappingShader = nullptr;
  iShaderAttribute *m_attrLayersDepth     = nullptr;
  iShaderAttribute *m_attrLayersBias    = nullptr;
  iShaderAttribute *m_attrShadowBuffers = nullptr;
  iShaderAttribute *m_attrDepthBuffer   = nullptr;


  GL4PSSMFilter m_shadowMapFilter;

  GfxSceneCollector m_collector;
  std::vector<GfxMesh *> m_meshesCache;
};


}


}

