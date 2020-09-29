#pragma once


#include <SpiceCore/coreexport.hh>
#include <SpiceCore/types.hh>
#include <SpiceCore/math/color4f.hh>
#include <SpiceCore/class.hh>


namespace Spice
{


SPICE_CLASS()
struct SPICE_CORE_API iGraphics : public SPICE_SUPER(iObject)
{
  SPICE_CLASS_GEN;

  virtual ~iGraphics(){}

  virtual void SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height) = 0;
  virtual void Clear(bool clearColor, const Color4f& color, bool clearDepth, float depth, bool clearStencil, UInt8 stencil) = 0;

};

}