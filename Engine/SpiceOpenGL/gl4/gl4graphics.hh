
#pragma once

#include <SpiceOpenGL/openglexport.hh>
#include <SpiceCore/class.hh>
#include <SpiceCore/graphics/igraphics.hh>

namespace Spice
{

SPICE_CLASS()
class SPICE_OGL_API GL4Graphics : public SPICE_SUPER(iGraphics)
{
  SPICE_CLASS_GEN_OBJECT;
public:
  GL4Graphics();
  virtual ~GL4Graphics();

  bool Initialize();

  void SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height);
  void Clear(bool clearColor, const Color4f & color, bool clearDepth, float depth, bool clearStencil, UInt8 stencil);

};


}