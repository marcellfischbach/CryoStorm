

#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/openglconstants.hh>
#include <ceOpenGL/gl4/pipeline/pssm/gl4pssmrenderer.hh>
#include <ceOpenGL/gl4/pipeline/pssm/gl4pssmfilter.hh>
#include <ceCore/csTypes.hh>
#include <ceCore/math/csMatrix4f.hh>
#include <map>
#include <array>
#include <vector>

namespace cryo
{

struct iDevice;
struct iSampler;
struct iShader;
struct iShaderAttribute;
struct iTexture2D;
struct iTexture2DArray;
class csCamera;
class csGfxMesh;
struct iGfxScene;
class csProjector;
class csSettings;

}

namespace cryo::opengl
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

  void Initialize();

  void SetDevice(iDevice *device);
  void SetScene(iGfxScene *scene);
  void SetDepthBuffer (iTexture2D* depthBuffer);

  void Clear();
  void Add(GL4DirectionalLight *directionalLight);

  void RenderShadowMaps(const csCamera &camera, const csProjector &projector);

  iTexture2D *GetColorTexture();
  iTexture2D *GetDepthTexture();



private:
  void SortLights();
  GL4RenderTarget2D *GetDirectionalLightShadowMap(size_t lightIdx);
  GL4PSSMShadowBufferObject *GetDirectionalLightShadowBuffer(size_t lightIdx);

  void RenderShadow(GL4DirectionalLight *directionalLight, const csCamera &camera, const csProjector &projector, size_t lightIdx);
  void ApplyShadowMapToDevice(GL4DirectionalLight* directionalLight, size_t lightIdx);


private:
  GL4Device *m_device = nullptr;
  iGfxScene *m_scene = nullptr;

  std::vector<GL4DirectionalLight *> m_shadowDirectionalLights;

  iTexture2D* m_depthBuffer = nullptr;

  std::array<GL4RenderTarget2D *, MaxLights> m_directionalLightShadowMap;
  std::array<GL4PSSMShadowBufferObject, MaxLights> m_directionalLightShadowBuffer;

  GL4PSSMRenderer m_pssmRenderer;
};

}