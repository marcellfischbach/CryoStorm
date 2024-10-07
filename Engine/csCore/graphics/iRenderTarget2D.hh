
#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/iRenderTarget.hh>
#include <csCore/graphics/iTexture2D.hh>
#include <csCore/graphics/iTexture2DArray.hh>
#include <csCore/graphics/iTextureCube.hh>

namespace cs
{

CS_CLASS()
struct CS_CORE_API iRenderTarget2D : public CS_SUPER(iRenderTarget)
{
  struct Descriptor
  {
    uint16_t Width;
    uint16_t Height;

  };

  CS_CLASS_GEN;
  ~iRenderTarget2D() override = default;

  virtual uint16_t GetWidth() const = 0;
  virtual uint16_t GetHeight() const = 0;

  virtual void SetDepthTexture(iTexture2D* depthTexture) = 0;
  virtual void SetDepthTexture(iTexture2DArray* depthTexture, size_t layer) = 0;
  virtual void SetDepthTexture(iTextureCube* depthTexture, eCubeFace face) = 0;
  virtual void SetDepthBuffer(ePixelFormat format) = 0;

  virtual void AddColorTexture(iTexture2D *colorTexture) = 0;
  virtual void AddColorTexture(iTexture2DArray *colorTexture, size_t layer) = 0;
  virtual void AddColorTexture(iTextureCube* colorTexture, eCubeFace face) = 0;

  virtual iTexture2D* GetDepthTexture() = 0;
  virtual iTextureCube* GetDepthTextureCube() = 0;
  virtual eCubeFace GetDepthTextureCubeFace() const = 0;
  virtual const iTexture2D* GetDepthTexture() const = 0;
  virtual const iTextureCube* GetDepthTextureCube() const = 0;

  virtual Size GetNumberOfColorTextures() const = 0;
  virtual iTexture2D* GetColorTexture(Size idx) = 0;
  virtual iTextureCube* GetColorTextureCube(Size idx) = 0;
  virtual eCubeFace GetColorTextureCubeFace(Size idx) const = 0;
  virtual const iTexture2D* GetColorTexture(Size idx) const = 0;
  virtual const iTextureCube* GetColorTextureCube(Size idx) const = 0;



};



}