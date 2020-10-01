
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/igraphics.hh>

namespace spc
{

SPC_CLASS()
class SPC_OGL_API GL4Graphics : public SPC_SUPER(iGraphics)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4Graphics();
  virtual ~GL4Graphics();

  bool Initialize();

  void SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height);
  void Clear(bool clearColor, const Color4f & color, bool clearDepth, float depth, bool clearStencil, UInt8 stencil);

};


}