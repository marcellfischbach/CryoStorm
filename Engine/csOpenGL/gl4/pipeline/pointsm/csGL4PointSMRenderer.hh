//
// Created by Marcell on 11.11.2023.
//

#pragma once
#include <csOpenGL/csOpenGLExport.hh>

#include <csOpenGL/gl4/pipeline/pointsm/csGL4PointSMFilter.hh>
#include <csCore/graphics/eCubeFace.hh>
#include <array>
#include <vector>

namespace cs
{

class csCamera;
class csGfxMesh;
class csProjector;
class csSettings;

struct iGfxScene;
struct iSampler;
struct iShader;
struct iShaderAttribute;
struct iTexture2D;
struct iTextureCube;

namespace opengl
{

class csGL4Device;
class csGL4PointLight;
class csGL4RenderTarget2D;
class csGL4RenderTargetCube;

class CS_OGL_API csGL4PointSMRenderer
{

public:
  csGL4PointSMRenderer();
  ~csGL4PointSMRenderer() = default;

  void Initialize();

  void SetDepthBuffer(iTexture2D *depthBuffer);
  void SetDevice(csGL4Device *device);
  void SetScene(iGfxScene *scene);
  csGL4RenderTarget2D *CreateShadowMap();
  void SetShadowMap(csGL4RenderTarget2D *shadowMap);
  csGL4RenderTarget2D *GetShadowMap();
  void RenderShadow(const csGL4PointLight *pointLight, const csCamera &camera, const csProjector &projector);

  bool IsShadowMapValid(csGL4RenderTarget2D *shadowMap) const;
  iTextureCube *GetShadowBufferColor();
  iTextureCube *GetShadowBufferDepth();
private:
  void RenderShadowBuffer(const csGL4PointLight *pointLight, const csCamera &camera, const csProjector &projector);
  void RenderShadowMap(const csGL4PointLight *pointLight, const csCamera &camera, const csProjector &projector);
  void FilterShadowMap();

  csGL4RenderTarget2D *GetShadowBuffer(eCubeFace face);

  csGL4RenderTarget2D *GetShadowMapTemp();

  iSampler *GetShadowMapColorSampler();
  iSampler *GetShadowBufferColorSampler();
  iSampler *GetShadowBufferDepthSampler();
private:
  csGL4Device *m_device = nullptr;
  iGfxScene   *m_scene  = nullptr;


  iTexture2D *m_depthBuffer = nullptr;

  iTextureCube *m_pointLightShadowBufferColor = nullptr;
  iTextureCube *m_pointLightShadowBufferDepth = nullptr;

  std::array<csGL4RenderTarget2D *, 6> m_pointLightShadowBuffer;
  size_t                               m_pointLightShadowBufferSize = 0;


  csGL4RenderTarget2D *m_pointLightShadowMapTemp = nullptr;
  csGL4RenderTarget2D *m_pointLightShadowMap     = nullptr;
  size_t              m_pointLightShadowMapWidth = 0;
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


  csGL4PointSMFilter m_shadowMapFilter;

  std::vector<csGfxMesh *> m_meshesCache;
};

} // ce
} // opengl
