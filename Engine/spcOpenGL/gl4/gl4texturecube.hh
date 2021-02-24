
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/itexturecube.hh>
#include <vector>


namespace spc::opengl
{

SPC_CLASS()
class SPC_OGL_API GL4TextureCube : public SPC_SUPER(iTextureCube)
{
SPC_CLASS_GEN_OBJECT;
public:
  GL4TextureCube();
  ~GL4TextureCube() override;

  SPC_NODISCARD eTextureType GetType() const override
  {
    return eTT_TextureCube;
  }

  ePixelFormat GetFormat() const override;

  void SetSampler(iSampler* sampler) override;
  SPC_NODISCARD iSampler* GetSampler() override;
  SPC_NODISCARD const iSampler* GetSampler() const override;

  void Bind();

  bool Initialize(UInt16 size, ePixelFormat format, bool generateMipMaps) override;
  void Data(eCubeFace face, const Image * image) override;
  void Data(eCubeFace face, UInt16 level, const Image * image) override;
  void Data(eCubeFace face, UInt16 level, ePixelFormat format, const void* data) override;
  void Data(eCubeFace face, UInt16 level, UInt16 x, UInt16 y, UInt16 width, UInt16 height, ePixelFormat format, const void* data) override;
private:
  UInt32 m_name;
  UInt16 m_size;
  ePixelFormat m_format;

  struct Level
  {
    UInt16 Size;
  };
  std::vector<Level> m_level;
  iSampler* m_sampler;
};

}