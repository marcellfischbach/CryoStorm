//
// Created by Marcell on 11.11.2023.
//

#pragma once

namespace ce
{

struct iGfxScene;
struct iTexture2D;

namespace opengl
{

class GL4Device;
class GL4RenderTarget2D;
class GL4RenderTargetCube;

class GL4PointSMRenderer
{
private:
  GL4Device *m_device = nullptr;
  iGfxScene *m_scene  = nullptr;


  iTexture2D *m_depthBuffer = nullptr;

  GL4RenderTargetCube *m_directionalLightShadowBuffer = nullptr;
  size_t               m_directionalLightShadowBufferSize = 0;


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
  iShaderAttribute *m_attrLayersBias      = nullptr;
  iShaderAttribute *m_attrShadowBuffer    = nullptr;
  iShaderAttribute *m_attrDepthBuffer     = nullptr;


  GL4DirectionalLightShadowMapFilter m_shadowMapFilter;

  std::vector<GfxMesh *> m_meshesCache;
};

} // ce
} // opengl
