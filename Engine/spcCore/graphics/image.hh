
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
  Image(uint16_t width, uint16_t height, ePixelFormat pixelFormat);

  enum class eMipMapProcedure
  {
    eMMP_Linear4x4,
    eMMP_Normal
  };
  void GenerateMipMaps(eMipMapProcedure procedure);

  void Copy(uint16_t layer, const uint8_t* buffer);
  
  uint16_t GetNumberOfLayers() const;
  uint16_t GetWidth(uint16_t layer = 0) const;
  uint16_t GetHeight(uint16_t layer = 0) const;
  const uint8_t* GetData(uint16_t layer = 0) const;
  ePixelFormat GetPixelFormat() const;



private:
  void GenerateLayers(uint16_t width, uint16_t height);
  void GenerateMipMapLayers();
  void GenerateLayer(uint16_t layer);
  void GenerateMipMapsLinear4x4();
  void GenerateMipMapsNormal();

  struct Layer
  {
    uint16_t width;
    uint16_t height;
    uint32_t size;
    uint8_t* buffer;
  };

  ePixelFormat m_pixelFormat;
  uint16_t m_numberOfLayers;
  Layer* m_layers;

  bool m_mipmapCreated;
  eMipMapProcedure m_mipMapProcedure;
};

}