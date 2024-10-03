
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>

#include <ceCore/defs.hh>
#include <ceCore/types.hh>
#include <ceCore/graphics/epixelformat.hh>


namespace ce
{

CS_CLASS()
class CS_CORE_API Image : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  Image(uint16_t width, uint16_t height, ePixelFormat pixelFormat);

  enum class eMipMapProcedure
  {
    eMMP_Linear4x4,
    eMMP_Normal
  };

  enum class eColorCorrection
  {
    eCC_Plain,
    eCC_Clamp3,
    eCC_Clamp4,
    eCC_Normalize
  };

  void GenerateMipMaps(eMipMapProcedure procedure);
  void ColorCorrection(eColorCorrection correctionMode);

  void Copy(uint16_t layer, const uint8_t* buffer);
  
  uint16_t GetNumberOfLayers() const;
  uint16_t GetWidth(uint16_t layer = 0) const;
  uint16_t GetHeight(uint16_t layer = 0) const;
  const uint8_t* GetData(uint16_t layer = 0) const;
  ePixelFormat GetPixelFormat() const;



private:
  void ColorCorrectionNormalize();
  void ColorCorrectionClamp3();
  void ColorCorrectionClamp4();

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