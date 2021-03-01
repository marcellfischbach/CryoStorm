
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/irendertarget.hh>
#include <spcCore/graphics/itexturecube.hh>

namespace spc
{

SPC_CLASS()
struct SPC_CORE_API iRenderTargetCube : public SPC_SUPER(iRenderTarget)
{
  struct Descriptor
  {
    UInt16 Size;

  };

  SPC_CLASS_GEN;
  ~iRenderTargetCube() override = default;

  virtual UInt16 GetSize() const = 0;

  virtual void SetDepthTexture(iTextureCube* depthTexture) = 0;

  virtual void AddColorTexture(iTextureCube*colorTexture) = 0;


  virtual iTextureCube* GetDepthTexture() = 0;
  virtual const iTextureCube* GetDepthTexture() const = 0;
  virtual Size GetNumberOfColorTextures() const = 0;
  virtual iTextureCube* GetColorTexture(Size idx) = 0;
  virtual const iTextureCube* GetColorTexture(Size idx) const = 0;



};



}