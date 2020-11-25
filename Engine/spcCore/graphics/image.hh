
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>

#include <spcCore/defs.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/epixelformat.hh>

namespace spc
{

SPC_CLASS()
class SPC_CORE_API Image : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN_OBJECT;
public:
  Image(UInt16 width, UInt16 height, ePixelFormat pixelFormat);

  void Copy(UInt16 layer, const UInt8* buffer);
  
  UInt16 GetNumberOfLayers() const;
  UInt16 GetWidth(UInt16 layer = 0) const;
  UInt16 GetHeight(UInt16 layer = 0) const;
  const UInt8* GetData(UInt16 layer = 0) const;
  ePixelFormat GetPixelFormat() const;



private:
  void GenerateLayers(UInt16 width, UInt16 height);
  void GenerateMipMapLayers();
  void GenerateLayer(UInt16 layer);

  struct Layer
  {
    UInt16 width;
    UInt16 height;
    UInt32 size;
    UInt8* buffer;
  };

  ePixelFormat m_pixelFormat;
  UInt16 m_numberOfLayers;
  Layer* m_layers;
};

}