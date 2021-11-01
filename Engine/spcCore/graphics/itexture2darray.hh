

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/epixelformat.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/types.hh>

namespace spc
{

class Image;
SPC_CLASS()
struct SPC_CORE_API iTexture2DArray : public SPC_SUPER(iTexture)
{

  struct Descriptor
  {
    ePixelFormat Format;
    uint16_t Width;
    uint16_t Height;
    uint16_t Layers;
    bool MipMaps;
  };


  SPC_CLASS_GEN;
  virtual ~iTexture2DArray() { }

  virtual void Data(uint16_t layer, const Image* image) = 0;
  virtual void Data(uint16_t layer, uint16_t level, const Image* image) = 0;
  virtual void Data(uint16_t layer, uint16_t level, ePixelFormat format, const void* data) = 0;
  virtual void Data(uint16_t layer, uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data) = 0;

};


}