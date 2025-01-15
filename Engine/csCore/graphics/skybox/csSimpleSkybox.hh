//
// Created by mcel on 28.05.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/iRenderTargetCube.hh>
#include <csCore/graphics/iSkyboxRenderer.hh>
#include <csCore/csRef.hh>

namespace cs
{

struct iShader;
struct iShaderAttribute;

CS_CLASS()
class CS_CORE_API csSimpleSkybox : public CS_SUPER(iSkyboxRenderer)
{
CS_CLASS_GEN_OBJECT;
public:
  csSimpleSkybox() = default;
  ~csSimpleSkybox() override = default;

  void Render(iDevice *device) override;
  iTextureCube *GetTexture() const override;

private:
  bool Prep(iDevice *device);
  bool PrepShader();
  bool PrepRenderTarget(cs::iDevice *device);

private:
  csAssetRef<iShader>      m_prepShader    = nullptr;
  csRef<iRenderTargetCube> m_skyboxTarget = nullptr;
  csAssetRef<iTextureCube> m_skyboxTexture = nullptr;

};

} // ce
