

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/epixelformat.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/types.hh>

namespace spc
{

class Image;
SPC_CLASS()
struct SPC_CORE_API iTexture2D : public SPC_SUPER(iTexture)
{

  struct Descriptor
  {
    ePixelFormat Format;
    UInt16 Width;
    UInt16 Height;
    bool MipMaps;
  };


  SPC_CLASS_GEN;
  virtual ~iTexture2D() { }

  virtual bool Initialize(UInt16 width, UInt16 height, ePixelFormat format, bool generateMipMaps) = 0;
  virtual void Data(const Image* image) = 0;
  virtual void Data(UInt8 level, const Image* image) = 0;
  virtual void Data(UInt8 level, ePixelFormat format, const void* data) = 0;
  virtual void Data(UInt8 level, UInt16 x, UInt16 y, UInt16 width, UInt16 height, ePixelFormat format, const void* data) = 0;

};


}