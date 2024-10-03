
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>


namespace cryo
{

struct iDevice;
struct iRenderPipeline;
struct iRenderTarget2D;

struct iGfxScene;
class GfxCamera;
class Camera;

class Projector;

CS_CLASS()
struct CS_CORE_API iFrameRenderer : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  ~iFrameRenderer() override = default;

  virtual void SetRenderPipeline(iRenderPipeline* renderPipeline) = 0;

  virtual void Render(iRenderTarget2D* target, iDevice* device, iGfxScene* scene) = 0;

  virtual void Render(iRenderTarget2D* target, GfxCamera *camera, iDevice* device, iGfxScene *scene) = 0;

};

}