
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>


namespace spc
{

struct iDevice;
struct iRenderTarget2D;

struct iGfxScene;
class GfxCamera;

class Camera;

class Projector;

SPC_CLASS()
struct SPC_CORE_API iFrameRenderer : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  ~iFrameRenderer() override = default;

  virtual void Render(iRenderTarget2D* target, iDevice* device, iGfxScene* scene) = 0;

  virtual void Render(iRenderTarget2D* target, GfxCamera *camera, iDevice* device, iGfxScene *scene) = 0;

};

}