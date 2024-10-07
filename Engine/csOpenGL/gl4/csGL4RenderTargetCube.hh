
#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/iRenderTargetCube.hh>

namespace cs::opengl
{

class csGL4TextureCube;

CS_CLASS()
class CS_OGL_API csGL4RenderTargetCube : public CS_SUPER(iRenderTargetCube)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4RenderTargetCube();
  virtual ~csGL4RenderTargetCube() override;

  void Bind();

  bool Initialize (uint16_t size);
  bool Initialize (uint32_t name, uint16_t size);

  uint16_t GetSize() const override;

  void SetDepthTexture(iTextureCube* depthTexture) override;
  void AddColorTexture(iTextureCube* colorTexture) override;

  CS_NODISCARD  eTextureType GetType() const override;

  bool Compile() override;
  std::string GetCompileLog() const override;

  iTextureCube* GetDepthTexture() override;
  const iTextureCube* GetDepthTexture() const override;
  Size GetNumberOfColorTextures() const override;
  iTextureCube* GetColorTexture(Size idx) override;
  const iTextureCube* GetColorTexture(Size idx) const override;

private:
  uint32_t m_name;
  uint16_t m_size;
  bool m_external;

  iTextureCube                   *m_depthTexture;
  std::vector<csGL4TextureCube*> m_colorTextures;

  std::string m_log;

};

}
