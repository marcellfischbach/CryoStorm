

#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/types.hh>
#include <spcCore/math/matrix4f.hh>
#include <map>
#include <vector>

namespace spc
{

struct iDevice;
struct iSampler;
struct iTexture2DArray;
class Camera;
class GfxScene;
class GfxMesh;
class Projector;
class Settings;

}

namespace spc::opengl
{

class GL4DirectionalLight;
class GL4RenderTarget2DArray;

class GL4ForwardDirectionalLightRenderer
{
public:
  GL4ForwardDirectionalLightRenderer();
  ~GL4ForwardDirectionalLightRenderer();

  void Initialize(Settings &settings);

  void SetDevice(iDevice* device);
  void SetScene(GfxScene* scene);


  void Clear();
  void Add(GL4DirectionalLight* directionalLight);

  Size RenderShadowMaps(Size maxShadowLights, const Camera &camera, const Projector &projector);

  iTexture2DArray* GetColorTexture();
  iTexture2DArray* GetDepthTexture();

private:
  void SortLights();
  GL4RenderTarget2DArray* GetDirectionalLightShadowMap(Size idx);
  iSampler* GetShadowMapColorSampler();
  iSampler* GetShadowMapDepthSampler();
  float GetSplitSize(const Vector3f* near, const Vector3f* far);

  void RenderDirectionalShadowMaps(GL4DirectionalLight* directionalLight, GL4RenderTarget2DArray* shadowMap, const Camera& camera, const Projector& projector);


private:
  iDevice* m_device;
  GfxScene* m_scene;

  std::vector<GL4DirectionalLight*> m_shadowDirectionalLights;

  std::vector< GL4RenderTarget2DArray*> m_directionalLightShadowMap;

  enum class ShadowMapFilter
  {
    Plain,
    PCF,
    VSM
  };

  Matrix4f m_shadowMatrices[3];
  Size m_directionalLightShadowMapSize;
  ShadowMapFilter m_shadowMapFilter;
  iSampler* m_shadowMapColorSampler;
  iSampler* m_shadowMapDepthSampler;


  std::vector<GfxMesh*> m_meshesCache;
};



}