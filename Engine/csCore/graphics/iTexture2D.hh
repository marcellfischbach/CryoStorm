

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/ePixelFormat.hh>
#include <csCore/graphics/iTexture.hh>
#include <csCore/csTypes.hh>

namespace cs
{

class csImage;
CS_CLASS(jclass="org.cryo.core.graphics.ITexture2D")
struct CS_CORE_API iTexture2D : public CS_SUPER(iTexture)
{

  struct Descriptor
  {
    ePixelFormat Format;
    uint16_t Width;
    uint16_t Height;
    bool MipMaps;
    uint16_t MultiSamples;
  };


  CS_CLASS_GEN;
  ~iTexture2D() override { }

  bool IsMultiSampling() const
  {
    return GetSamples() > 1;
  }

  virtual uint16_t GetSamples() const = 0;
  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  virtual void Data(const csImage* image) = 0;
  virtual void Data(uint16_t level, const csImage* image) = 0;
  virtual void Data(uint16_t level, ePixelFormat format, const void* data) = 0;
  virtual void Data(uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data) = 0;

};


}