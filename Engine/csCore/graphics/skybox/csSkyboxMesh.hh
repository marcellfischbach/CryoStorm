#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csRef.hh>

namespace cs
{

class csCamera;
class csProjector;
struct iDevice;
struct iRenderMesh;
struct iShader;
struct iShaderAttribute;
struct iTexture2D;
struct iTextureCube;

class CS_CORE_API csSkyboxMesh
{
public:
  csSkyboxMesh() = default;

  void Render (iDevice* device, float size, iTextureCube *texture, iTexture2D *depth);
private:
  void RenderDeferred (iDevice* device, float size, iTextureCube *texture, iTexture2D *depth);
  void RenderForward (iDevice* device, float size, iTextureCube *texture);

  iRenderMesh *RenderMesh (iDevice * device);
  iShader* ShaderDeferred(iDevice *device);
  iShader* ShaderForward(iDevice *device);
  iRenderMesh* m_renderMesh = nullptr;

  csAssetRef<iShader> m_shaderDeferred;
  iShaderAttribute *m_attrDeferredSkybox      = nullptr;
  iShaderAttribute *m_attrDeferredRenderPlane = nullptr;
  iShaderAttribute *m_attrDeferredDepth      = nullptr;

  csAssetRef<iShader> m_shaderForward;
  iShaderAttribute *m_attrForwardSkybox      = nullptr;
  iShaderAttribute *m_attrForwardRenderPlane = nullptr;
};

} // ce
