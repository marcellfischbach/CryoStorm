//
// Created by MCEL on 06.11.2023.
//

#pragma once

#include <vector>
#include <ceOpenGL/gl4/pipeline/pssm/gl4directionallightshadowmapfilter.hh>

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

class GL4PSSMRenderer
{
public:
  GL4PSSMRenderer();
  ~GL4PSSMRenderer();

  void Initialize(Settings &settings);

  void SetDepthBuffer (iTexture2D *depthBuffer);
  void SetDevice (GL4Device *device);
  void SetScene (iGfxScene *scene);
  GL4RenderTarget2D *CreateDirectionalLightShadowMap();
  void SetShadowMap (GL4RenderTarget2D *shadowMap);
  GL4RenderTarget2D *GetShadowMap ();

  void RenderShadow(GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);

  bool IsShadowMapValid (GL4RenderTarget2D *shadowMap);
private:
  void RenderShadowBuffer(GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);
  void RenderShadowMap(GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);
  void FilterShadowMap();


  GL4RenderTarget2D *GetDirectionalLightShadowMapTemp();
  GL4RenderTarget2DArray *GetDirectionalLightShadowBuffer();
  float GetSplitSize(const Vector3f *near, const Vector3f *far);
  iSampler *GetShadowMapColorSampler();
  iSampler *GetShadowBufferColorSampler();
  iSampler *GetShadowBufferDepthSampler();

private:
  GL4Device *m_device = nullptr;
  iGfxScene *m_scene  = nullptr;


  iTexture2D *m_depthBuffer = nullptr;

  GL4RenderTarget2DArray *m_directionalLightShadowBuffer = nullptr;
  size_t                 m_directionalLightShadowBufferSize;


  GL4RenderTarget2D *m_directionalLightShadowMapTemp = nullptr;
  GL4RenderTarget2D *m_directionalLightShadowMap;
  size_t            m_directionalLightShadowMapWidth;
  size_t            m_directionalLightShadowMapHeight;

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
  iShaderAttribute *m_attrLayersBias      = nullptr;
  iShaderAttribute *m_attrShadowBuffer    = nullptr;
  iShaderAttribute *m_attrDepthBuffer     = nullptr;


  GL4DirectionalLightShadowMapFilter m_shadowMapFilter;

  std::vector<GfxMesh *> m_meshesCache;
};


}


}

