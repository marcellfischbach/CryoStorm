
#pragma once

#include <csOpenGL/csOpenGLExport.hh>

#include <csOpenGL/gl4/pipeline/pointsm/csGL4PointSMFilter.hh>
#include <csCore/graphics/eCubeFace.hh>
#include <csCore/csRef.hh>
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
  csOwned<csGL4RenderTarget2D> CreateShadowMap();
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


  csAssetRef<iTexture2D> m_depthBuffer;

  csAssetRef<iTextureCube> m_pointLightShadowBufferColor;
  csAssetRef<iTextureCube> m_pointLightShadowBufferDepth;

  std::array<csRef<csGL4RenderTarget2D>, 6> m_pointLightShadowBuffer;
  size_t                               m_pointLightShadowBufferSize = 0;


  csRef<csGL4RenderTarget2D> m_pointLightShadowMapTemp = nullptr;
  csRef<csGL4RenderTarget2D> m_pointLightShadowMap     = nullptr;
  size_t              m_pointLightShadowMapWidth = 0;
  size_t            m_pointLightShadowMapHeight = 0;

  enum class ShadowSamplingMode
  {
    Plain,
    PCF,
    VSM
  };

  ShadowSamplingMode m_shadowSamplingMode;
  csAssetRef<iSampler> m_shadowMapColorSampler;
  csAssetRef<iSampler> m_shadowBufferColorSampler;
  csAssetRef<iSampler> m_shadowMapDepthSampler;

  csAssetRef<iShader> m_shadowMappingShader;
  iShaderAttribute *m_attrLightPosition   = nullptr;
  iShaderAttribute *m_attrMappingBias     = nullptr;
  iShaderAttribute *m_attrShadowBuffer    = nullptr;
  iShaderAttribute *m_attrDepthBuffer     = nullptr;


  csGL4PointSMFilter m_shadowMapFilter;

  std::vector<csGfxMesh *> m_meshesCache;
};

} // ce
} // opengl
