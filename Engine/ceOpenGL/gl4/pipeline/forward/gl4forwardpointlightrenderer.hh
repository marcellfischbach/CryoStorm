

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/openglconstants.hh>
#include <ceCore/types.hh>
#include <map>
#include <vector>
#include <array>

namespace ce
{

struct iDevice;
struct iSampler;
struct iShader;
struct iShaderAttribute;
struct iGfxScene;
class Settings;

class Camera;
class Projector;

}

namespace ce::opengl
{

class GL4PointLight;
class GL4RenderTarget2D;
class GL4RenderTargetCube;

class GL4ForwardPointLightRenderer
{
public:
  GL4ForwardPointLightRenderer();
  ~GL4ForwardPointLightRenderer();

  void Initialize(Settings &settings);

  void SetDevice(iDevice* device);
  void SetScene(iGfxScene* scene);


  void Clear();
  void Add(GL4PointLight* pointLight);

  void RenderShadowMaps(const Camera &camera,
                        const Projector &projector);

private:
  void SortLights();
  GL4RenderTargetCube* GetPointLightShadowMap(Size idx);
  iSampler* GetShadowMapColorSampler();
  iSampler* GetShadowMapDepthSampler();
  void RenderPointShadowMaps(GL4PointLight* pointLight, GL4RenderTargetCube* shadowMap);
  void RenderPointShadowMapsStraight(GL4PointLight* pointLight, GL4RenderTargetCube* shadowMap);



  void RenderShadow(GL4PointLight *pointLight, const Camera &camera, const Projector &projector, size_t lightIdx);
  void RenderShadowBuffer(GL4PointLight *pointLight, const Camera &camera, const Projector &projector);
  void RenderShadowMap(GL4PointLight *pointLight, const Camera &camera, const Projector &projector);
  void FilterShadowMap(size_t lightIdx);
  void ApplyShadowMapToDevice(GL4PointLight* pointLight, size_t lightIdx);


private:
  iDevice        * m_device;
  iGfxScene * m_scene;

  std::vector<GL4PointLight*> m_shadowPointLights;
  std::vector< GL4RenderTargetCube*> m_pointLightShadowMap;


  std::array<GL4RenderTarget2D *, MaxLights> m_pointLightShadowMap;

  iShader *m_shadowMappingShader = nullptr;
  iShaderAttribute *m_attrLightPosition = nullptr;
  iShaderAttribute *m_attrMappingBias = nullptr;
  iShaderAttribute *m_attrShadowBuffer = nullptr;
  iShaderAttribute *m_attrDepthBuffer = nullptr;


  enum class ShadowMapFilter
  {
    Plain,
    PCF,
    VSM
  };
  Size m_pointLightShadowBufferSize;
  ShadowMapFilter m_shadowMapFilter;
  iSampler* m_shadowMapColorSampler;
  iSampler* m_shadowMapDepthSampler;

  unsigned m_shadowFBO = 0;

};



}