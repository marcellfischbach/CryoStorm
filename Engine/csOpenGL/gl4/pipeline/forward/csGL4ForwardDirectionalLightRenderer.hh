

#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csOpenGL/csOpenglConstants.hh>
#include <csOpenGL/gl4/pipeline/pssm/csGL4PSSMRenderer.hh>
#include <csOpenGL/gl4/pipeline/pssm/csGL4PSSMFilter.hh>
#include <csCore/csTypes.hh>
#include <csCore/math/csMatrix4f.hh>
#include <map>
#include <array>
#include <vector>

namespace cs
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

namespace cs::opengl
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
  iPSSMShadowBufferObject *GetDirectionalLightShadowBuffer(size_t lightIdx);

  void RenderShadow(csGL4DirectionalLight *directionalLight, const csCamera &camera, const csProjector &projector, size_t lightIdx);
  void ApplyShadowMapToDevice(csGL4DirectionalLight* directionalLight, size_t lightIdx);


private:
  csRef<csGL4Device> m_device;
  csRef<iGfxScene> m_scene;

  std::vector<csGL4DirectionalLight *> m_shadowDirectionalLights;

  csAssetRef<iTexture2D> m_depthBuffer;

  std::array<csRef<csGL4RenderTarget2D>, MaxLights>       m_directionalLightShadowMap;
  std::array<iPSSMShadowBufferObject*, MaxLights> m_directionalLightShadowBuffer;

  csGL4PSSMRenderer m_pssmRenderer;
};

}