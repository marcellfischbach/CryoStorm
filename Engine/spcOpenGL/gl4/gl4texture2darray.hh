
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/itexture2darray.hh>
#include <vector>


namespace spc::opengl
{

SPC_CLASS()
class SPC_OGL_API GL4Texture2DArray : public SPC_SUPER(iTexture2DArray)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4Texture2DArray();
  ~GL4Texture2DArray();
  SPC_NODISCARD UInt32 GetName()
  {
    return m_name;
  }

  SPC_NODISCARD eTextureType GetType() const
  {
    return eTT_Texture2DArray;
  }

  ePixelFormat GetFormat() const override;

  void SetSampler(iSampler* sampler) override;
  iSampler* GetSampler() override;
  const iSampler* GetSampler() const override;

  void Bind();

  bool Initialize(UInt16 width, UInt16 height, UInt16 layer, ePixelFormat format, bool generateMipMaps);

  void Data(UInt16 layer, const Image * image) override;
  void Data(UInt16 layer, UInt16 level, const Image * image) override;
  void Data(UInt16 layer, UInt16 level, ePixelFormat format, const void* data) override;
  void Data(UInt16 layer, UInt16 level, UInt16 x, UInt16 y, UInt16 width, UInt16 height, ePixelFormat format, const void* data) override;
private:
  UInt32 m_name;
  UInt16 m_width;
  UInt16 m_height;
  UInt16 m_layer;
  ePixelFormat m_format;

  struct Level
  {
    UInt16 Width;
    UInt16 Height;
  };
  std::vector<Level> m_level;
  iSampler* m_sampler;
};

}