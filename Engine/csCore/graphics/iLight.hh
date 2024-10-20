

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/math/csColor4f.hh>
#include <csCore/graphics/eLightChangeMode.hh>
#include <csCore/graphics/eLightType.hh>

namespace cs
{


CS_CLASS(jclass="org.cryo.core.graphics.ILight")
struct CS_CORE_API iLight : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iLight () override = default;

  CS_FUNCTION(jenum)
  CS_NODISCARD virtual cs::eLightType GetType() const = 0;

  CS_FUNCTION(jenum)
  virtual void SetChangeMode(cs::eLightChangeMode changeMode) = 0;
  CS_FUNCTION(jenum)
  CS_NODISCARD virtual cs::eLightChangeMode GetChangeMode() const = 0;

  CS_FUNCTION(jenum="cs::eLightChangeMode,cs::eLightType")
  virtual void SetValue(cs::eLightChangeMode changeMode, cs::eLightType type) {}

  CS_FUNCTION()
  virtual void SetCastShadow(bool castShadow) = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual bool IsCastShadow() const = 0;

  CS_FUNCTION()
  virtual void SetShadowMapBias(float bias) = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual float GetShadowMapBias() const = 0;

  CS_FUNCTION()
  virtual void SetColor(const cs::csColor4f& color) = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual const cs::csColor4f& GetColor() const = 0;

  CS_FUNCTION()
  virtual void SetIntensity(float intensity) = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual float GetIntensity() const = 0;

};

}