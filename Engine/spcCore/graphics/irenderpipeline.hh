
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

namespace spc
{

struct iDevice;
class GfxScene;
class Camera;
class Projector;

SPC_CLASS()
struct SPC_CORE_API iRenderPipeline : public SPC_SUPER(iObject)
{
SPC_CLASS_GEN;

  ~iRenderPipeline() override = default;

  virtual void Render(Camera& camera, Projector& projector, iDevice* device, GfxScene* scene) = 0;
};


}