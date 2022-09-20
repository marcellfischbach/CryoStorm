

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
  GL4RenderTarget2D *GetDirectionalLightShadowMap(size_t lightIdx);
  GL4RenderTarget2DArray *GetDirectionalLightShadowBuffer();
  iSampler *GetShadowMapColorSampler();
  iSampler *GetShadowMapDepthSampler();
  float GetSplitSize(const Vector3f *near, const Vector3f *far);

  void RenderShadow(GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector, size_t lightIdx);
  void RenderShadowBuffer(GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector);
  void RenderShadowMap(GL4DirectionalLight *directionalLight, const Camera &camera, const Projector &projector, size_t lightIdx);
  void ApplyShadowMapToDevice(GL4DirectionalLight* directionalLight, size_t lightIdx);


private:
  GL4Device *m_device;
  iGfxScene *m_scene;


  std::vector<GL4DirectionalLight *> m_shadowDirectionalLights;

  iTexture2D* m_depthBuffer;

  GL4RenderTarget2DArray *m_directionalLightShadowBuffer;
  size_t m_directionalLightShadowBufferSize;


  std::vector<GL4RenderTarget2D *> m_directionalLightShadowMap;
  size_t m_directionalLightShadowMapWidth;
  size_t m_directionalLightShadowMapHeight;

  enum class ShadowMapFilter
  {
    Plain,
    PCF,
    VSM
  };

  float m_shadowNear;
  float m_shadowFar;
  float m_splits[3];

  Matrix4f m_shadowMatrices[3];
  ShadowMapFilter m_shadowMapFilter;
  iSampler *m_shadowMapColorSampler;
  iSampler *m_shadowMapDepthSampler;

  iShader *m_shadowMappingShader;
  iShaderAttribute *m_attrLayersBias;
  iShaderAttribute *m_attrMappingMatrices;
  iShaderAttribute *m_attrShadowBuffer;
  iShaderAttribute *m_attrDepthBuffer;

  std::vector<GfxMesh *> m_meshesCache;
};

}