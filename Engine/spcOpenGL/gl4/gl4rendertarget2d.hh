
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/irendertarget2d.hh>

namespace spc::opengl
{

class GL4Texture2D;

SPC_CLASS()
class SPC_OGL_API GL4RenderTarget2D : public SPC_SUPER(iRenderTarget2D)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderTarget2D();
  virtual ~GL4RenderTarget2D() override;

  void Bind();

  bool Initialize (uint16_t width, uint16_t height);

  uint16_t GetWidth() const override;
  uint16_t GetHeight() const override;

  void SetDepthTexture(iTexture2D* depthTexture) override;
  void SetDepthBuffer(ePixelFormat format) override;
  void AddColorTexture(iTexture2D* colorTexture) override;

  SPC_NODISCARD  eTextureType GetType() const override;

  bool Compile() override;
  std::string GetCompileLog() const override;

  iTexture2D* GetDepthTexture() override;
  const iTexture2D* GetDepthTexture() const override;
  Size GetNumberOfColorTextures() const override;
  iTexture2D* GetColorTexture(Size idx) override;
  const iTexture2D* GetColorTexture(Size idx) const override;

private:
  uint32_t m_name;
  uint16_t m_width;
  uint16_t m_height;

  uint32_t  m_depthBuffer;
  iTexture2D *m_depthTexture;
  std::vector<GL4Texture2D*> m_colorTextures;

  std::string m_log;
};

}
