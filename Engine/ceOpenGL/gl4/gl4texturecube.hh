
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/itexturecube.hh>
#include <vector>


namespace ce::opengl
{

CE_CLASS()
class CE_OGL_API GL4TextureCube : public CE_SUPER(iTextureCube)
{
CE_CLASS_GEN_OBJECT;
public:
  GL4TextureCube();
  ~GL4TextureCube() override;
  CE_NODISCARD uint32_t GetName()
  {
    return m_name;
  }
  CE_NODISCARD eTextureType GetType() const override
  {
    return eTT_TextureCube;
  }

  ePixelFormat GetFormat() const override;

  void SetSampler(iSampler* sampler) override;
  CE_NODISCARD iSampler* GetSampler() override;
  CE_NODISCARD const iSampler* GetSampler() const override;

  void Bind();
  void Unbind();

  bool Initialize(uint16_t size, ePixelFormat format, bool generateMipMaps) override;
  void Data(eCubeFace face, const Image * image) override;
  void Data(eCubeFace face, uint16_t level, const Image * image) override;
  void Data(eCubeFace face, uint16_t level, ePixelFormat format, const void* data) override;
  void Data(eCubeFace face, uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data) override;
private:
  uint32_t m_name;
  uint16_t m_size;
  ePixelFormat m_format;

  struct Level
  {
    uint16_t Size;
  };
  std::vector<Level> m_level;
  iSampler* m_sampler;
};

}