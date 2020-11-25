
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/graphics/itexture2d.hh>
#include <spcCore/graphics/etextureunit.hh>

namespace spc
{

SPC_CLASS()
class SPC_OGL_API GL4Device : public SPC_SUPER(iDevice)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4Device();
  virtual ~GL4Device();

  bool Initialize();

  void SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height) override;
  void Clear(bool clearColor, const Color4f & color, bool clearDepth, float depth, bool clearStencil, UInt8 stencil) override;

  void SetShader(iShader * shader) override;

  iTexture2D* CreateTexture(const iTexture2D::Descriptor & descriptor) override;
  void ResetTextures() override;
  eTextureUnit BindTexture(iTexture * texture) override;

private:

  eTextureUnit ShiftTextureUnit();

  eTextureUnit m_nextTextureUnit;
};


}