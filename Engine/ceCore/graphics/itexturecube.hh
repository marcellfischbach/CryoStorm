

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/epixelformat.hh>
#include <ceCore/graphics/itexture.hh>
#include <ceCore/types.hh>
#include <ceCore/graphics/ecubeface.hh>

namespace ce
{

class Image;
CS_CLASS()
struct CS_CORE_API iTextureCube : public CS_SUPER(iTexture)
{

  struct Descriptor
  {
    ePixelFormat Format;
    uint16_t Size;
    bool MipMaps;
  };


  CS_CLASS_GEN;
  virtual ~iTextureCube() = default;

  virtual bool Initialize(uint16_t size, ePixelFormat format, bool generateMipMaps) = 0;
  virtual void Data(eCubeFace face, const Image* image) = 0;
  virtual void Data(eCubeFace face, uint16_t level, const Image* image) = 0;
  virtual void Data(eCubeFace face, uint16_t level, ePixelFormat format, const void* data) = 0;
  virtual void Data(eCubeFace face, uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data) = 0;

};


}