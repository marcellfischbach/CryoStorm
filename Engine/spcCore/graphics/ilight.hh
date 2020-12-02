

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/graphics/elightchangemode.hh>

namespace spc
{


SPC_CLASS()
struct SPC_CORE_API iLight : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  virtual ~iLight () { }

  virtual void SetChangeMode(eLightChangeMode changeMode) = 0;
  virtual eLightChangeMode GetChangeMode() const = 0;

  virtual void SetCastShadow(bool castShadow) = 0;
  virtual bool IsCastShadow() const = 0;

  virtual void SetColor(const Color4f& color) = 0;
  virtual const Color4f& GetColor() const = 0;


};

}