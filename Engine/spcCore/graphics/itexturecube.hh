

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/epixelformat.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/ecubeface.hh>

namespace spc
{

class Image;
SPC_CLASS()
struct SPC_CORE_API iTextureCube : public SPC_SUPER(iTexture)
{

  struct Descriptor
  {
    ePixelFormat Format;
    uint16_t Size;
    bool MipMaps;
  };


  SPC_CLASS_GEN;
  virtual ~iTextureCube() = default;

  virtual bool Initialize(uint16_t size, ePixelFormat format, bool generateMipMaps) = 0;
  virtual void Data(eCubeFace face, const Image* image) = 0;
  virtual void Data(eCubeFace face, uint16_t level, const Image* image) = 0;
  virtual void Data(eCubeFace face, uint16_t level, ePixelFormat format, const void* data) = 0;
  virtual void Data(eCubeFace face, uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data) = 0;

};


}