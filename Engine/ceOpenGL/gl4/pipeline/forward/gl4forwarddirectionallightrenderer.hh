

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/types.hh>
#include <ceCore/math/matrix4f.hh>
#include <map>
#include <vector>

namespace ce
{

struct iDevice;
struct iSampler;
struct iShader;
struct iTexture2DArray;
class Camera;
class GfxMesh;
struct iGfxScene;
class Projector;
class Settings;

}

namespace ce::opengl
{

class GL4DirectionalLight;
class GL4RenderTarget2DArray;

class GL4ForwardDirectionalLightRenderer
{
public:
  GL4ForwardDirectionalLightRenderer();
  ~GL4ForwardDirectionalLightRenderer();

  void Initialize(Settings& settings);

  void SetDevice(iDevice* device);
  void SetScene(iGfxScene* scene);

  void Clear();
  void Add(GL4DirectionalLight* directionalLight);

  void RenderShadowMaps(const Camera& camera, const Projector& projector);

  iTexture2DArray* GetColorTexture();
  iTexture2DArray* GetDepthTexture();

private:
  void SortLights();
  GL4RenderTarget2DArray* GetDirectionalLightShadowBuffer();
  iSampler* GetShadowMapColorSampler();
  iSampler* GetShadowMapDepthSampler();
  float GetSplitSize(const Vector3f* near, const Vector3f* far);

  void RenderShadow(GL4DirectionalLight* directionalLight, const Camera& camera, const Projector& projector);
  void RenderShadowBuffer(GL4DirectionalLight* directionalLight, const Camera& camera, const Projector& projector);
  void RenderDirectionalShadowMaps(GL4DirectionalLight* directionalLight,
                                   GL4RenderTarget2DArray* shadowMap,
                                   const Camera& camera,
                                   const Projector& projector);


private:
  iDevice  * m_device;
  iGfxScene* m_scene;

  std::vector<GL4DirectionalLight*> m_shadowDirectionalLights;

  GL4RenderTarget2DArray* m_directionalLightShadowBuffer;
  Size m_directionalLightShadowBufferSize;

  enum class ShadowMapFilter
  {
    Plain,
    PCF,
    VSM
  };

  float m_shadowNear;
  float m_shadowFar;
  float m_splits[3];

  Matrix4f        m_shadowMatrices[3];
  ShadowMapFilter m_shadowMapFilter;
  iSampler* m_shadowMapColorSampler;
  iSampler* m_shadowMapDepthSampler;

  iShader* m_shadowMappingShader;

  std::vector<GfxMesh*> m_meshesCache;
};

}