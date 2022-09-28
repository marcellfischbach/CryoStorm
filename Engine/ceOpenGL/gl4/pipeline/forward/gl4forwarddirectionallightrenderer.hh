

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/openglconstants.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwarddirectionallightshadowmapfilter.hh>
#include <ceCore/types.hh>
#include <ceCore/math/matrix4f.hh>
#include <map>
#include <array>
#include <vector>

namespace ce
{

struct iDevice;
struct iSampler;
struct iShader;
struct iShaderAttribute;
struct iTexture2D;
struct iTexture2DArray;
class Camera;
class GfxMesh;
struct iGfxScene;
class Projector;
class Settings;

}

namespace ce::opengl
{

class GL4Device;
class GL4DirectionalLight;
class GL4RenderTarget2D;
class GL4RenderTarget2DArray;

class GL4ForwardDirectionalLightRenderer
{
public:
  GL4ForwardDirectionalLightRenderer();
  ~GL4ForwardDirectionalLightRenderer();

  void Initialize(Settings &settings);

  void SetDevice(iDevice *device);
  void SetScene(iGfxScene *scene);
  void SetDepthBuffer (iTexture2D* depthBuffer);
  void SetShadowMapSize (size_t width, size_t height);

  void Clear();
  void Add(GL4DirectionalLight *directionalLight);

  void RenderShadowMaps(const Camera &camera, const Projector &projector);

  iTexture2D *GetColorTexture();
  iTexture2D *GetDepthTexture();



private:
  void SortLights();
  GL4RenderTarget2D *GetDirectionalLightShadowMapTemp();
  GL4RenderTarget2D *GetDirectionalLightShadowMap(size_t lightIdx);
  GL4RenderTarget2D *CreateDirectionalLightShadowMap();
  GL4RenderTarget2DArray *GetDirectionalLightShadowBuffer();
  iSampler *GetShadowMapColorSampler();
  iSampler *GetShadowBufferColorSampler();
  iSampler *GetShadowBufferDepthSampler();
  float GetSplitSize(const Vector3f *near, const Vector3f *far);

  void RenderShadow(GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector, size_t lightIdx);
  void RenderShadowBuffer(GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);
  void RenderShadowMap(GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);
  void FilterShadowMap(size_t lightIdx);
  void ApplyShadowMapToDevice(GL4DirectionalLight* directionalLight, size_t lightIdx);


private:
  GL4Device *m_device = nullptr;
  iGfxScene *m_scene = nullptr;


  std::vector<GL4DirectionalLight *> m_shadowDirectionalLights;

  iTexture2D* m_depthBuffer = nullptr;

  GL4RenderTarget2DArray *m_directionalLightShadowBuffer = nullptr;
  size_t m_directionalLightShadowBufferSize;


  GL4RenderTarget2D * m_directionalLightShadowMapTemp = nullptr;
  std::array<GL4RenderTarget2D *, MaxLights> m_directionalLightShadowMap;
  size_t m_directionalLightShadowMapWidth;
  size_t m_directionalLightShadowMapHeight;

  enum class ShadowSamplingMode
  {
    Plain,
    PCF,
    VSM
  };

  float m_shadowNear;
  float m_shadowFar;
  float m_splits[3];

  ShadowSamplingMode m_shadowSamplingMode;
  iSampler *m_shadowMapColorSampler = nullptr;
  iSampler *m_shadowBufferColorSampler = nullptr;
  iSampler *m_shadowMapDepthSampler = nullptr;

  iShader *m_shadowMappingShader = nullptr;
  iShaderAttribute *m_attrLayersBias = nullptr;
  iShaderAttribute *m_attrShadowBuffer = nullptr;
  iShaderAttribute *m_attrDepthBuffer = nullptr;


  GL4ForwardDirectionalLightShadowMapFilter m_shadowMapFilter;

  std::vector<GfxMesh *> m_meshesCache;
};

}