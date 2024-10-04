

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/graphics/ePixelFormat.hh>
#include <ceCore/graphics/iTexture.hh>
#include <ceCore/csTypes.hh>

namespace cryo
{

class csImage;
CS_CLASS()
struct CS_CORE_API iTexture2DArray : public CS_SUPER(iTexture)
{

  struct Descriptor
  {
    ePixelFormat Format;
    uint16_t Width;
    uint16_t Height;
    uint16_t Layers;
    bool MipMaps;
  };


  CS_CLASS_GEN;
  virtual ~iTexture2DArray() { }

  virtual void Data(uint16_t layer, const csImage* image) = 0;
  virtual void Data(uint16_t layer, uint16_t level, const csImage* image) = 0;
  virtual void Data(uint16_t layer, uint16_t level, ePixelFormat format, const void* data) = 0;
  virtual void Data(uint16_t layer, uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data) = 0;

};


}