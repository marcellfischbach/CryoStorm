

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/graphics/ePixelFormat.hh>
#include <ceCore/graphics/iTexture.hh>
#include <ceCore/csTypes.hh>
#include <ceCore/graphics/eCubeFace.hh>

namespace cryo
{

class csImage;
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
  virtual void Data(eCubeFace face, const csImage* image) = 0;
  virtual void Data(eCubeFace face, uint16_t level, const csImage* image) = 0;
  virtual void Data(eCubeFace face, uint16_t level, ePixelFormat format, const void* data) = 0;
  virtual void Data(eCubeFace face, uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data) = 0;

};


}