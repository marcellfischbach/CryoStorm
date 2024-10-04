
#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/iTextureCube.hh>
#include <vector>


namespace cryo::opengl
{

CS_CLASS()
class CS_OGL_API csGL4TextureCube : public CS_SUPER(iTextureCube)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4TextureCube();
  ~csGL4TextureCube() override;
  CS_NODISCARD uint32_t GetName()
  {
    return m_name;
  }
  CS_NODISCARD eTextureType GetType() const override
  {
    return eTextureType::TextureCube;
  }

  ePixelFormat GetFormat() const override;

  void SetSampler(iSampler* sampler) override;
  CS_NODISCARD iSampler* GetSampler() override;
  CS_NODISCARD const iSampler* GetSampler() const override;

  void Bind();
  void Unbind();

  bool Initialize(uint16_t size, ePixelFormat format, bool generateMipMaps) override;
  void Data(eCubeFace face, const csImage * image) override;
  void Data(eCubeFace face, uint16_t level, const csImage * image) override;
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