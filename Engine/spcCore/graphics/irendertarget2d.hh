
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/irendertarget.hh>
#include <spcCore/graphics/itexture2d.hh>

namespace spc
{

SPC_CLASS()
struct SPC_CORE_API iRenderTarget2D : public SPC_SUPER(iRenderTarget)
{
  struct Descriptor
  {
    UInt16 Width;
    UInt16 Height;

  };

  SPC_CLASS_GEN;
  ~iRenderTarget2D() override = default;

  virtual UInt16 GetWidth() const = 0;
  virtual UInt16 GetHeight() const = 0;

  virtual void SetDepthTexture(iTexture2D* depthTexture) = 0;
  virtual void SetDepthBuffer(ePixelFormat format) = 0;

  virtual void AddColorTexture(iTexture2D *colorTexture) = 0;

  virtual Size GetNumberOfColorTextures() const = 0;
  virtual iTexture2D* GetColorTexture(Size idx) = 0;
  virtual const iTexture2D* GetColorTexture(Size idx) const = 0;



};



}