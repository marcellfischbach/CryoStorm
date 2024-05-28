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
struct iTextureCube;

class CE_CORE_API SkyboxMesh
{
public:
  SkyboxMesh() = default;

  void Render (iDevice* device, float size, iTextureCube *texture);
private:
  iRenderMesh *RenderMesh (iDevice * device);
  iShader* Shader(iDevice *device);
  iRenderMesh* m_renderMesh = nullptr;

  iShader* m_shader = nullptr;
  iShaderAttribute *m_attrSkybox = nullptr;
  iShaderAttribute *m_attrRenderPlane = nullptr;
};

} // ce
