#pragma once

#include <ceCore/coreexport.hh>

namespace ce
{

class Camera;
class Projector;
struct iDevice;
struct iRenderMesh;
struct iShader;
struct iShaderAttribute;
struct iTexture2D;
struct iTextureCube;

class CE_CORE_API SkyboxMesh
{
public:
  SkyboxMesh() = default;

  void Render (iDevice* device, float size, iTextureCube *texture, iTexture2D *depth);
private:
  iRenderMesh *RenderMesh (iDevice * device);
  iShader* Shader(iDevice *device);
  iRenderMesh* m_renderMesh = nullptr;

  iShader* m_shader = nullptr;
  iShaderAttribute *m_attrSkybox = nullptr;
  iShaderAttribute *m_attrRenderPlane = nullptr;
  iShaderAttribute *m_attrDepth = nullptr;
};

} // ce
