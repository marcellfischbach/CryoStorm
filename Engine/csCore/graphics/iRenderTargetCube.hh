
#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/iRenderTarget.hh>
#include <csCore/graphics/iTextureCube.hh>

namespace cs
{

CS_CLASS()
struct CS_CORE_API iRenderTargetCube : public CS_SUPER(iRenderTarget)
{
  struct Descriptor
  {
    uint16_t Size;

  };

  CS_CLASS_GEN;
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