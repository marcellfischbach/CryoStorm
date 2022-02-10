
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/irendertarget.hh>
#include <ceCore/graphics/itexturecube.hh>

namespace ce
{

CE_CLASS()
struct CE_CORE_API iRenderTargetCube : public CE_SUPER(iRenderTarget)
{
  struct Descriptor
  {
    uint16_t Size;

  };

  CE_CLASS_GEN;
  ~iRenderTargetCube() override = default;

  virtual uint16_t GetSize() const = 0;

  virtual void SetDepthTexture(iTextureCube* depthTexture) = 0;

  virtual void AddColorTexture(iTextureCube*colorTexture) = 0;


  virtual iTextureCube* GetDepthTexture() = 0;
  virtual const iTextureCube* GetDepthTexture() const = 0;
  virtual Size GetNumberOfColorTextures() const = 0;
  virtual iTextureCube* GetColorTexture(Size idx) = 0;
  virtual const iTextureCube* GetColorTexture(Size idx) const = 0;



};



}