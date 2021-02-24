

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
    UInt16 Size;
    bool MipMaps;
  };


  SPC_CLASS_GEN;
  virtual ~iTextureCube() = default;

  virtual bool Initialize(UInt16 size, ePixelFormat format, bool generateMipMaps) = 0;
  virtual void Data(eCubeFace face, const Image* image) = 0;
  virtual void Data(eCubeFace face, UInt16 level, const Image* image) = 0;
  virtual void Data(eCubeFace face, UInt16 level, ePixelFormat format, const void* data) = 0;
  virtual void Data(eCubeFace face, UInt16 level, UInt16 x, UInt16 y, UInt16 width, UInt16 height, ePixelFormat format, const void* data) = 0;

};


}