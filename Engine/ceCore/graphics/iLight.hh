

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/csColor4f.hh>
#include <ceCore/graphics/eLightChangeMode.hh>
#include <ceCore/graphics/eLightType.hh>

namespace cryo
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

  virtual void SetColor(const csColor4f& color) = 0;
  virtual CS_NODISCARD const csColor4f& GetColor() const = 0;

  virtual void SetIntensity(float intensity) = 0;
  virtual CS_NODISCARD float GetIntensity() const = 0;

};

}