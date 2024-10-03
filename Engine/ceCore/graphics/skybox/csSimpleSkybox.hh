//
// Created by mcel on 28.05.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/iRenderTarget2D.hh>
#include <ceCore/graphics/iRenderTargetCube.hh>
#include <ceCore/graphics/iSkyboxRenderer.hh>

namespace cryo
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
  bool PrepRenderTarget(cryo::iDevice *device);

private:
  iShader           *m_prepShader    = nullptr;
  iRenderTargetCube *m_skyboxTarget  = nullptr;
  iTextureCube      *m_skyboxTexture = nullptr;

};

} // ce
