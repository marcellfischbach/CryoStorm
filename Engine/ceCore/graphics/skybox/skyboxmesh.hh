#pragma once

#include <ceCore/coreexport.hh>

namespace ce
{

struct iDevice;
struct iRenderMesh;
struct iShader;
struct iShaderAttribute;
struct iTextureCube;

class CE_CORE_API SkyboxMesh
{
public:
  SkyboxMesh() = default;

  void Render (iDevice* device, iTextureCube *texture);
private:
  iRenderMesh *RenderMesh (iDevice * device);
  iShader* Shader(iDevice *device);
  iRenderMesh* m_renderMesh = nullptr;

  iShader* m_shader = nullptr;
  iShaderAttribute *m_attrSkybox = nullptr;
};

} // ce
