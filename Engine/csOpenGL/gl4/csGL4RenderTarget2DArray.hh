
#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/iRenderTarget2DArray.hh>
#include <csCore/csRef.hh>

namespace cs::opengl
{

class csGL4Texture2D;

CS_CLASS()
class CS_OGL_API csGL4RenderTarget2DArray : public CS_SUPER(iRenderTarget2DArray)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4RenderTarget2DArray();
  virtual ~csGL4RenderTarget2DArray() override;

  void Bind();

  bool Initialize (uint16_t width, uint16_t height, uint16_t layer);

  uint16_t GetWidth() const override;
  uint16_t GetHeight() const override;
  uint16_t GetLayer() const override;

  void SetDepthTexture(iTexture2DArray* depthTexture) override;
  void AddColorTexture(iTexture2DArray* colorTexture) override;

  CS_NODISCARD  eTextureType GetType() const override;

  bool Compile() override;
  std::string GetCompileLog() const override;

  iTexture2DArray* GetDepthTexture() override;
  const iTexture2DArray* GetDepthTexture() const override;
  Size GetNumberOfColorTextures() const override;
  iTexture2DArray* GetColorTexture(Size idx) override;
  const iTexture2DArray* GetColorTexture(Size idx) const override;

private:
  uint32_t m_name;
  uint16_t m_width;
  uint16_t m_height;
  uint16_t m_layer;

  csAssetRef<iTexture2DArray> m_depthTexture;
  std::vector<csAssetRef<iTexture2DArray>> m_colorTextures;

  std::string m_log;
};

}
