

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/graphics/elightchangemode.hh>
#include <ceCore/graphics/elighttype.hh>

namespace ce
{


CE_CLASS()
struct CE_CORE_API iLight : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;

  virtual ~iLight () { }

  virtual CE_NODISCARD eLightType GetType() const = 0;

  virtual void SetChangeMode(eLightChangeMode changeMode) = 0;
  virtual CE_NODISCARD eLightChangeMode GetChangeMode() const = 0;

  virtual void SetCastShadow(bool castShadow) = 0;
  virtual CE_NODISCARD bool IsCastShadow() const = 0;

  virtual void SetShadowMapBias(float bias) = 0;
  virtual CE_NODISCARD float GetShadowMapBias() const = 0;

  virtual void SetColor(const Color4f& color) = 0;
  virtual CE_NODISCARD const Color4f& GetColor() const = 0;

  virtual void SetIntensity(float intensity) = 0;
  virtual CE_NODISCARD float GetIntensity() const = 0;

};

}