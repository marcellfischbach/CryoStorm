#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/types.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/class.hh>


namespace spc
{


SPC_CLASS()
struct SPC_CORE_API iGraphics : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  virtual ~iGraphics(){}

  virtual void SetViewport(Int16 x, Int16 y, UInt16 width, UInt16 height) = 0;
  virtual void Clear(bool clearColor, const Color4f& color, bool clearDepth, float depth, bool clearStencil, UInt8 stencil) = 0;

};

}