
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/itexture2d.hh>
#include <vector>


namespace spc::opengl
{

SPC_CLASS()
class SPC_OGL_API GL4Texture2D : public SPC_SUPER(iTexture2D)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4Texture2D();
  ~GL4Texture2D();

  eTextureType GetType() const
  {
    return eTT_Texture2D;
  }

  void SetSampler(iSampler* sampler) override;
  iSampler* GetSampler() override;
  const iSampler* GetSampler() const override;

  void Bind();

  bool Initialize(UInt16 width, UInt16 height, ePixelFormat format, bool generateMipMaps) override;
  void Data(UInt8 level, const Image * image) override;
  void Data(UInt8 level, ePixelFormat format, const void* data) override;
  void Data(UInt8 level, UInt16 x, UInt16 y, UInt16 width, UInt16 height, ePixelFormat format, const void* data) override;
private:
  UInt32 m_name;
  UInt16 m_width;
  UInt16 m_height;

  struct Level
  {
    UInt16 Width;
    UInt16 Height;
  };
  std::vector<Level> m_level;
  iSampler* m_sampler;
};

}