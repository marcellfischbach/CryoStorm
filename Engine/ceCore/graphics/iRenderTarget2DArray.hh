
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/iRenderTarget.hh>
#include <ceCore/graphics/iTexture2DArray.hh>

namespace cryo
{

CS_CLASS()
struct CS_CORE_API iRenderTarget2DArray : public CS_SUPER(iRenderTarget)
{
  struct Descriptor
  {
    uint16_t Width;
    uint16_t Height;
    uint16_t Layer;
  };

  CS_CLASS_GEN;
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