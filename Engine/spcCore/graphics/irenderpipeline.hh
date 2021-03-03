
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
{

struct iDevice;
struct iRenderTarget2D;
class GfxScene;
class Camera;
class Projector;

SPC_CLASS()
struct SPC_CORE_API iRenderPipeline : public SPC_SUPER(iObject)
{
SPC_CLASS_GEN;

  ~iRenderPipeline() override = default;

  virtual void Initialize() = 0;

  virtual void Render(iRenderTarget2D* target, Camera& camera, Projector& projector, iDevice* device, GfxScene* scene) = 0;
};


}