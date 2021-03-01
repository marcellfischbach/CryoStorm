

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/graphics/elightchangemode.hh>
#include <spcCore/graphics/elighttype.hh>

namespace spc
{


SPC_CLASS()
struct SPC_CORE_API iLight : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  virtual ~iLight () { }

  virtual SPC_NODISCARD eLightType GetType() const = 0;

  virtual void SetChangeMode(eLightChangeMode changeMode) = 0;
  virtual SPC_NODISCARD eLightChangeMode GetChangeMode() const = 0;

  virtual void SetCastShadow(bool castShadow) = 0;
  virtual SPC_NODISCARD bool IsCastShadow() const = 0;

  virtual void SetColor(const Color4f& color) = 0;
  virtual SPC_NODISCARD const Color4f& GetColor() const = 0;

  virtual SPC_NODISCARD float GetIntensity() const = 0;

};

}