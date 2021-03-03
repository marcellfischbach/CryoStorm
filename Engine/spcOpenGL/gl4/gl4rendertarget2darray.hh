
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/irendertarget2darray.hh>

namespace spc::opengl
{

class GL4Texture2D;

SPC_CLASS()
class SPC_OGL_API GL4RenderTarget2DArray : public SPC_SUPER(iRenderTarget2DArray)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderTarget2DArray();
  virtual ~GL4RenderTarget2DArray() override;

  void Bind();

  bool Initialize (UInt16 width, UInt16 height, UInt16 layer);

  UInt16 GetWidth() const override;
  UInt16 GetHeight() const override;
  UInt16 GetLayer() const override;

  void SetDepthTexture(iTexture2DArray* depthTexture) override;
  void AddColorTexture(iTexture2DArray* colorTexture) override;

  SPC_NODISCARD  eTextureType GetType() const override;

  bool Compile() override;
  std::string GetCompileLog() const override;

  iTexture2DArray* GetDepthTexture() override;
  const iTexture2DArray* GetDepthTexture() const override;
  Size GetNumberOfColorTextures() const override;
  iTexture2DArray* GetColorTexture(Size idx) override;
  const iTexture2DArray* GetColorTexture(Size idx) const override;

private:
  UInt32 m_name;
  UInt16 m_width;
  UInt16 m_height;
  UInt16 m_layer;

  iTexture2DArray*m_depthTexture;
  std::vector<iTexture2DArray*> m_colorTextures;

  std::string m_log;
};

}
