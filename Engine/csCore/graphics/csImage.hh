
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>

#include <csCore/csDefs.hh>
#include <csCore/csTypes.hh>
#include <csCore/graphics/ePixelFormat.hh>
#include <csCore/resource/iAsset.hh>


namespace cs
{

CS_CLASS()
class CS_CORE_API csImage : public CS_SUPER(iAsset)
{
  CS_CLASS_GEN_OBJECT;
  CS_ASSET_GEN;
public:
  csImage(uint16_t width, uint16_t height, ePixelFormat pixelFormat);

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