

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/graphics/elightchangemode.hh>
#include <ceCore/graphics/elighttype.hh>

namespace ce
{


CS_CLASS()
struct CS_CORE_API iLight : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  virtual ~iLight () { }

  virtual CS_NODISCARD eLightType GetType() const = 0;

  virtual void SetChangeMode(eLightChangeMode changeMode) = 0;
  virtual CS_NODISCARD eLightChangeMode GetChangeMode() const = 0;

  virtual void SetCastShadow(bool castShadow) = 0;
  virtual CS_NODISCARD bool IsCastShadow() const = 0;

  virtual void SetShadowMapBias(float bias) = 0;
  virtual CS_NODISCARD float GetShadowMapBias() const = 0;

  virtual void SetColor(const Color4f& color) = 0;
  virtual CS_NODISCARD const Color4f& GetColor() const = 0;

  virtual void SetIntensity(float intensity) = 0;
  virtual CS_NODISCARD float GetIntensity() const = 0;

};

}