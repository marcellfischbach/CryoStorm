
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>

namespace ce
{

struct iDevice;
struct iRenderTarget2D;
class GfxCamera;
struct iGfxScene;

CE_CLASS()
struct CE_CORE_API iRenderPipeline : public CE_SUPER(iObject)
{
CE_CLASS_GEN;

  ~iRenderPipeline() override = default;

  virtual void Initialize() = 0;

  virtual void Render(iRenderTarget2D* target, const GfxCamera* camera, iDevice* device, iGfxScene* scene) = 0;
};


}