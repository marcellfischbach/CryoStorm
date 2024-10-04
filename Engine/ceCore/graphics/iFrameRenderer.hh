
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/csClass.hh>


namespace cryo
{

struct iDevice;
struct iRenderPipeline;
struct iRenderTarget2D;

struct iGfxScene;
class csGfxCamera;
class csCamera;

class csProjector;

CS_CLASS()
struct CS_CORE_API iFrameRenderer : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  ~iFrameRenderer() override = default;

  virtual void SetRenderPipeline(iRenderPipeline* renderPipeline) = 0;

  virtual void Render(iRenderTarget2D* target, iDevice* device, iGfxScene* scene) = 0;

  virtual void Render(iRenderTarget2D* target, csGfxCamera *camera, iDevice* device, iGfxScene *scene) = 0;

};

}