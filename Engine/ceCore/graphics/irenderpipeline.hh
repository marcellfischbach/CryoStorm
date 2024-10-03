
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>

namespace cryo
{

struct iDevice;
struct iRenderTarget2D;
class GfxCamera;
struct iGfxScene;

CS_CLASS()
struct CS_CORE_API iRenderPipeline : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

  ~iRenderPipeline() override = default;

  virtual void Initialize() = 0;

  virtual void Render(iRenderTarget2D* target, const GfxCamera* camera, iDevice* device, iGfxScene* scene) = 0;
};


}