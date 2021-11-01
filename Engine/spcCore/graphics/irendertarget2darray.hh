
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/irendertarget.hh>
#include <spcCore/graphics/itexture2darray.hh>

namespace spc
{

SPC_CLASS()
struct SPC_CORE_API iRenderTarget2DArray : public SPC_SUPER(iRenderTarget)
{
  struct Descriptor
  {
    uint16_t Width;
    uint16_t Height;
    uint16_t Layer;
  };

  SPC_CLASS_GEN;
  ~iRenderTarget2DArray() override = default;

  virtual uint16_t GetWidth() const = 0;
  virtual uint16_t GetHeight() const = 0;
  virtual uint16_t GetLayer() const = 0;

  virtual void SetDepthTexture(iTexture2DArray* depthTexture) = 0;
  virtual void AddColorTexture(iTexture2DArray*colorTexture) = 0;

  virtual iTexture2DArray* GetDepthTexture() = 0;
  virtual const iTexture2DArray* GetDepthTexture() const = 0;
  virtual Size GetNumberOfColorTextures() const = 0;
  virtual iTexture2DArray* GetColorTexture(Size idx) = 0;
  virtual const iTexture2DArray* GetColorTexture(Size idx) const = 0;



};



}