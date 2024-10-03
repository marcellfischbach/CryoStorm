
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/itexture2d.hh>
#include <vector>


namespace ce::opengl
{

CS_CLASS()
class CS_OGL_API GL4Texture2D : public CS_SUPER(iTexture2D)
{
  CS_CLASS_GEN_OBJECT;
public:
  GL4Texture2D();
  ~GL4Texture2D() override;
  CS_NODISCARD uint32_t GetName()
  {
    return m_name;
  }

  CS_NODISCARD eTextureType GetType() const
  {
    return eTextureType::Texture2D;
  }

  ePixelFormat GetFormat() const override;

  void SetSampler(iSampler* sampler) override;
  iSampler* GetSampler() override;
  const iSampler* GetSampler() const override;

  void Bind();
  void Unbind();

  uint32_t GetWidth() const override { return m_width; }
  uint32_t GetHeight() const override { return m_height; }

  bool Initialize(uint16_t width, uint16_t height, ePixelFormat format, bool generateMipMaps, uint16_t multiSamples);

  uint16_t GetSamples() const override;
  void Data(const Image * image) override;
  void Data(uint16_t level, const Image * image) override;
  void Data(uint16_t level, ePixelFormat format, const void* data) override;
  void Data(uint16_t level, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ePixelFormat format, const void* data) override;
private:
  uint32_t m_target;
  uint32_t m_name;
  uint16_t m_width;
  uint16_t m_height;
  ePixelFormat m_format;
  uint16_t m_samples;
  bool  m_multiSampling;

  struct Level
  {
    uint16_t Width;
    uint16_t Height;
  };
  std::vector<Level> m_level;
  iSampler* m_sampler;
};

}