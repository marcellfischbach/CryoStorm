
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>

namespace cs
{

struct iDevice;
struct iRenderTarget2D;
struct iTexture2DArray;
class csGfxCamera;
struct iGfxScene;

CS_CLASS()
struct CS_CORE_API iRenderPipeline : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

  ~iRenderPipeline() override = default;

  virtual void Initialize() = 0;
  virtual void Shutdown() = 0;

  virtual void Render(iRenderTarget2D* target, const csGfxCamera* camera, iDevice* device, iGfxScene* scene) = 0;
};


CS_CLASS(Virtual)
struct CS_CORE_API iDeferredRenderPipeline : public CS_SUPER(iRenderPipeline)
{
  CS_CLASS_GEN;
  ~iDeferredRenderPipeline() override = default;

  virtual const iTexture2DArray *GetPSSMShadowBufferDepth () const = 0;
  virtual const iTexture2DArray *GetPSSMShadowBufferDepth (size_t lightIndex) const = 0;
  virtual const iTexture2DArray *GetPSSMShadowBufferColor () const = 0;
  virtual const iTexture2DArray *GetPSSMShadowBufferColor (size_t lightIndex) const = 0;
};


CS_CLASS(Virtual)
struct CS_CORE_API iForwardRenderPipeline : public CS_SUPER(iRenderPipeline)
{
CS_CLASS_GEN;
  ~iForwardRenderPipeline() override = default;
};

}