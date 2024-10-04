

#pragma once

#include <ceOpenGL/csOpenGLExport.hh>
#include <ceOpenGL/csOpenglConstants.hh>
#include <ceOpenGL/gl4/pipeline/pssm/csGL4PSSMRenderer.hh>
#include <ceOpenGL/gl4/pipeline/pssm/csGL4PSSMFilter.hh>
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

class csGL4Device;
class csGL4DirectionalLight;
class csGL4RenderTarget2D;
class csGL4RenderTarget2DArray;

class csGL4ForwardDirectionalLightRenderer
{
public:
  csGL4ForwardDirectionalLightRenderer();
  ~csGL4ForwardDirectionalLightRenderer();

  void Initialize();

  void SetDevice(iDevice *device);
  void SetScene(iGfxScene *scene);
  void SetDepthBuffer (iTexture2D* depthBuffer);

  void Clear();
  void Add(csGL4DirectionalLight *directionalLight);

  void RenderShadowMaps(const csCamera &camera, const csProjector &projector);

  iTexture2D *GetColorTexture();
  iTexture2D *GetDepthTexture();



private:
  void SortLights();
  csGL4RenderTarget2D *GetDirectionalLightShadowMap(size_t lightIdx);
  csGL4PSSMShadowBufferObject *GetDirectionalLightShadowBuffer(size_t lightIdx);

  void RenderShadow(csGL4DirectionalLight *directionalLight, const csCamera &camera, const csProjector &projector, size_t lightIdx);
  void ApplyShadowMapToDevice(csGL4DirectionalLight* directionalLight, size_t lightIdx);


private:
  csGL4Device *m_device = nullptr;
  iGfxScene   *m_scene  = nullptr;

  std::vector<csGL4DirectionalLight *> m_shadowDirectionalLights;

  iTexture2D* m_depthBuffer = nullptr;

  std::array<csGL4RenderTarget2D *, MaxLights>       m_directionalLightShadowMap;
  std::array<csGL4PSSMShadowBufferObject, MaxLights> m_directionalLightShadowBuffer;

  csGL4PSSMRenderer m_pssmRenderer;
};

}