#pragma once

#include <csOpenGL/gl4/pipeline/pssm/csGL4PSSMFilter.hh>
#include <csOpenGL/gl4/pipeline/pssm/iPSSMRenderer.hh>
#include <csCore/graphics/scene/csGfxSceneCollector.hh>
#include <csCore/math/csMatrix4f.hh>
#include <csCore/csRef.hh>

#include <array>
#include <vector>


namespace cs
{

struct iGfxScene;
struct iSampler;
struct iShader;
struct iShaderAttribute;
struct iTexture2D;
class csCamera;
class csGfxMesh;
class csProjector;
class csSettings;

namespace opengl
{

class csGL4Device;
class csGL4DirectionalLight;
class csGL4RenderTarget2DArray;
class csGL4RenderTarget2D;
class csGL4Texture2DArray;


struct csGL4PSSMShadowBufferObject : public iPSSMShadowBufferObject
{
  csRef<csGL4Texture2DArray>                ShadowDepth;
  csRef<csGL4Texture2DArray>                ShadowColor;
  std::array<csRef<csGL4RenderTarget2D>, 4> ShadowBuffers = {nullptr, nullptr, nullptr, nullptr};

  csGL4PSSMShadowBufferObject();
  csGL4PSSMShadowBufferObject(const csGL4PSSMShadowBufferObject &sbo);
  csGL4PSSMShadowBufferObject(csGL4PSSMShadowBufferObject &&sbo);
  ~csGL4PSSMShadowBufferObject();

  csGL4PSSMShadowBufferObject &operator=(const csGL4PSSMShadowBufferObject &sbo);

};

class csGL4PSSMRenderer : public iPSSMRenderer
{
public:
  csGL4PSSMRenderer();
  ~csGL4PSSMRenderer() = default;

  void Initialize();

  void SetDepthBuffer(iTexture2D *depthBuffer) override;
  void SetDevice(csGL4Device *device) override;
  void SetScene(iGfxScene *scene) override;
  csOwned<csGL4RenderTarget2D> CreateDirectionalLightShadowMap() override;

  void SetShadowMap(csGL4RenderTarget2D *shadowMap) override;
  csGL4RenderTarget2D *GetShadowMap() override;


  csGL4PSSMShadowBufferObject *CreateDirectionalLightShadowBuffer() override;
  void DeleteDirectionalLightShadowBuffer(iPSSMShadowBufferObject *sbo) override;

  void SetShadowBuffer(iPSSMShadowBufferObject *shadowBuffer) override;
  csGL4PSSMShadowBufferObject *GetShadowBuffer() override;
  csGL4RenderTarget2D *GetShadowBuffer(size_t splitLayer) override;

  void RenderShadow(const csGL4DirectionalLight *directionalLight,
                    const csCamera &camera,
                    const csProjector &projector) override;

  bool IsShadowMapValid(csGL4RenderTarget2D *shadowMap) const override;
  bool IsShadowBufferValid(iPSSMShadowBufferObject *shadowMap) const override;


private:
  void
  RenderShadowBuffer(const csGL4DirectionalLight *directionalLight,
                     const csCamera &camera,
                     const csProjector &projector);
  void
  RenderShadowMap(const csGL4DirectionalLight *directionalLight, const csCamera &camera, const csProjector &projector);
  void FilterShadowMap();


  csGL4RenderTarget2D *GetDirectionalLightShadowMapTemp();
  static float GetSplitSize(const csVector3f *near, const csVector3f *far);
  iSampler *GetShadowMapColorSampler();
  iSampler *GetShadowBufferColorSampler();
  iSampler *GetShadowBufferDepthSampler();


private:
  csRef<csGL4Device> m_device = nullptr;
  csRef<iGfxScene>   m_scene  = nullptr;


  csAssetRef<iTexture2D> m_depthBuffer = nullptr;

  csGL4PSSMShadowBufferObject *m_directionalLightShadowBuffers;
  size_t                      m_directionalLightShadowBufferSize = 0;


  csRef<csGL4RenderTarget2D> m_directionalLightShadowMapTemp   = nullptr;
  csRef<csGL4RenderTarget2D> m_directionalLightShadowMap       = nullptr;
  size_t                     m_directionalLightShadowMapWidth  = 0;
  size_t                     m_directionalLightShadowMapHeight = 0;

  enum class ShadowSamplingMode
  {
    Plain,
    PCF,
    VSM
  };

  float                m_shadowNear;
  float                m_shadowFar;
  std::array<float, 4> m_splits;

  std::array<csMatrix4f, 4> m_shadowMapViewProjection;

  ShadowSamplingMode   m_shadowSamplingMode;
  csAssetRef<iSampler> m_shadowMapColorSampler;
  csAssetRef<iSampler> m_shadowBufferColorSampler;
  csAssetRef<iSampler> m_shadowMapDepthSampler;

  csAssetRef<iShader> m_shadowMappingShader;
  iShaderAttribute    *m_attrLayersDepth                   = nullptr;
  iShaderAttribute    *m_attrLayersBias                    = nullptr;
  iShaderAttribute    *m_attrShadowBuffers                 = nullptr;
  iShaderAttribute    *m_attrShadowBufferDatas             = nullptr;
  iShaderAttribute    *m_attrDepthBuffer                   = nullptr;
  iShaderAttribute    *m_attrShadowMapViewProjectionMatrix = nullptr;


  csGL4PSSMFilter m_shadowMapFilter;

  csGfxSceneCollector m_collector;
};


}


}

