

#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/types.hh>
#include <map>
#include <vector>

namespace spc
{

struct iDevice;
struct iSampler;
class GfxScene;
class Settings;

}

namespace spc::opengl
{

class GL4PointLight;
class GL4RenderTargetCube;

class GL4ForwardPointLightRenderer
{
public:
  GL4ForwardPointLightRenderer();
  ~GL4ForwardPointLightRenderer();

  void Initialize(Settings &settings);

  void SetDevice(iDevice* device);
  void SetScene(GfxScene* scene);


  void Clear();
  void Add(GL4PointLight* pointLight);

  Size RenderShadowMaps(Size maxShadowLights);

private:
  void SortLights();
  GL4RenderTargetCube* GetPointLightShadowMap(Size idx);
  iSampler* GetShadowMapColorSampler();
  iSampler* GetShadowMapDepthSampler();
  void RenderPointShadowMaps(GL4PointLight* pointLight, GL4RenderTargetCube* shadowMap);
  void RenderPointShadowMapsStraight(GL4PointLight* pointLight, GL4RenderTargetCube* shadowMap);

private:
  iDevice* m_device;
  GfxScene* m_scene;

  std::vector<GL4PointLight*> m_shadowPointLights;

  std::vector< GL4RenderTargetCube*> m_pointLightShadowMap;

  enum class ShadowMapFilter
  {
    Plain,
    PCF,
    VSM
  };
  Size m_pointLightShadowMapSize;
  ShadowMapFilter m_shadowMapFilter;
  iSampler* m_shadowMapColorSampler;
  iSampler* m_shadowMapDepthSampler;

};



}