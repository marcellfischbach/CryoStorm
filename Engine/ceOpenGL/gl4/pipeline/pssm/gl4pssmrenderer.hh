//
// Created by MCEL on 06.11.2023.
//

#pragma once

#include <ceOpenGL/gl4/pipeline/pssm/gl4pssmfilter.hh>
#include <ceCore/graphics/scene/gfxscenecollector.hh>
#include <ceCore/math/matrix4f.hh>

#include <array>
#include <vector>


namespace cryo
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


struct GL4PSSMShadowBufferObject
{
  GL4Texture2DArray                  *ShadowDepth = nullptr;
  GL4Texture2DArray                  *ShadowColor = nullptr;
  std::array<GL4RenderTarget2D *, 4> ShadowBuffers = { nullptr, nullptr, nullptr, nullptr };

  GL4PSSMShadowBufferObject();
  GL4PSSMShadowBufferObject(const GL4PSSMShadowBufferObject &sbo);
  GL4PSSMShadowBufferObject(GL4PSSMShadowBufferObject &&sbo);
  ~GL4PSSMShadowBufferObject();

  GL4PSSMShadowBufferObject &operator=(const GL4PSSMShadowBufferObject &sbo);

};

class GL4PSSMRenderer
{
public:
  GL4PSSMRenderer();
  ~GL4PSSMRenderer() = default;

  void Initialize();

  void SetDepthBuffer(iTexture2D *depthBuffer);
  void SetDevice(GL4Device *device);
  void SetScene(iGfxScene *scene);
  GL4RenderTarget2D *CreateDirectionalLightShadowMap();

  void SetShadowMap(GL4RenderTarget2D *shadowMap);
  GL4RenderTarget2D *GetShadowMap();


  GL4PSSMShadowBufferObject CreateDirectionalLightShadowBuffer();

  void SetShadowBuffer(const GL4PSSMShadowBufferObject &shadowBuffer);
  const GL4PSSMShadowBufferObject &GetShadowBuffer();
  GL4RenderTarget2D *GetShadowBuffer(size_t splitLayer);

  void RenderShadow(const GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);

  bool IsShadowMapValid(GL4RenderTarget2D *shadowMap) const;
  bool IsShadowBufferValid(GL4PSSMShadowBufferObject &shadowMap) const;

  CS_NODISCARD const std::array<Matrix4f, 4> &GetMatrices() const;
  CS_NODISCARD const std::array<float, 4> &GetSplits() const;

private:
  void
  RenderShadowBuffer(const GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);
  void RenderShadowMap(const GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);
  void FilterShadowMap();


  GL4RenderTarget2D *GetDirectionalLightShadowMapTemp();
  static float GetSplitSize(const Vector3f *near, const Vector3f *far);
  iSampler *GetShadowMapColorSampler();
  iSampler *GetShadowBufferColorSampler();
  iSampler *GetShadowBufferDepthSampler();


private:
  GL4Device *m_device = nullptr;
  iGfxScene *m_scene  = nullptr;


  iTexture2D *m_depthBuffer = nullptr;

  GL4PSSMShadowBufferObject m_directionalLightShadowBuffers;
  size_t                    m_directionalLightShadowBufferSize = 0;


  GL4RenderTarget2D *m_directionalLightShadowMapTemp  = nullptr;
  GL4RenderTarget2D *m_directionalLightShadowMap      = nullptr;
  size_t            m_directionalLightShadowMapWidth  = 0;
  size_t            m_directionalLightShadowMapHeight = 0;

  enum class ShadowSamplingMode
  {
    Plain,
    PCF,
    VSM
  };

  float                m_shadowNear;
  float                m_shadowFar;
  std::array<float, 4> m_splits;

  std::array<Matrix4f, 4> m_shadowMatrices;

  ShadowSamplingMode m_shadowSamplingMode;
  iSampler           *m_shadowMapColorSampler    = nullptr;
  iSampler           *m_shadowBufferColorSampler = nullptr;
  iSampler           *m_shadowMapDepthSampler    = nullptr;

  iShader          *m_shadowMappingShader = nullptr;
  iShaderAttribute *m_attrLayersDepth     = nullptr;
  iShaderAttribute *m_attrLayersBias      = nullptr;
  iShaderAttribute *m_attrShadowBuffers   = nullptr;
  iShaderAttribute *m_attrDepthBuffer     = nullptr;


  GL4PSSMFilter m_shadowMapFilter;

  GfxSceneCollector      m_collector;
  std::vector<GfxMesh *> m_meshesCache;
};


}


}

