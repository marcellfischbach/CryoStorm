
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
#include <csCore/graphics/eRenderPipeline.hh>


namespace cs
{

struct iDevice;
struct iRenderPipeline;
struct iRenderTarget2D;

struct iGfxScene;
class csGfxCamera;
class csCamera;

class csProjector;

CS_CLASS(jclass="org.cryo.core.graphics.IFrameRenderer")
struct CS_CORE_API iFrameRenderer : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  ~iFrameRenderer() override = default;

  virtual void Render(iRenderTarget2D* target, iDevice* device, iGfxScene* scene) = 0;

  virtual void Render(iRenderTarget2D* target, csGfxCamera *camera, iDevice* device, iGfxScene *scene) = 0;

};


CS_CLASS()
struct CS_CORE_API iFrameRendererFactory : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  virtual csOwned<iFrameRenderer> Create (eRenderPipeline pipeline) const = 0;
};


}