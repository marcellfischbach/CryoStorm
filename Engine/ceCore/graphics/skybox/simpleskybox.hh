//
// Created by mcel on 28.05.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/irendertargetcube.hh>
#include <ceCore/graphics/iskyboxrenderer.hh>

namespace ce
{

struct iShader;
struct iShaderAttribute;

CE_CLASS()
class CE_CORE_API SimpleSkybox : public CE_SUPER(iSkyboxRenderer)
{
CE_CLASS_GEN_OBJECT;
public:
  SimpleSkybox() = default;
  ~SimpleSkybox() override = default;

  void Render(iDevice *device) override;
  iTextureCube *GetTexture() const override;

private:
  bool Prep(iDevice *device);
  bool PrepShader();
  bool PrepRenderTarget(ce::iDevice *device);

private:
  iShader           *m_prepShader    = nullptr;
  iRenderTargetCube *m_skyboxTarget  = nullptr;
  iTextureCube      *m_skyboxTexture = nullptr;

};

} // ce
