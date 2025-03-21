
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include <csCore/csTypes.hh>
#include <csCore/csClass.hh>
#include <csCore/input/eMouseButton.hh>
#include <csCore/input/eCursorMode.hh>

namespace cs
{


CS_CLASS(jclass="org.cryo.core.input.IMouse")
struct CS_CORE_API iMouse : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  iMouse () = default;
  ~iMouse() override = default;

  CS_FUNCTION()
  virtual void SetVisible (bool visible) = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual bool IsVisible () const = 0;

  virtual void SetCursorMode (eCursorMode mode) = 0;
  CS_NODISCARD virtual eCursorMode GetCursorMode () const = 0;


  CS_FUNCTION()
  CS_NODISCARD virtual int32_t GetX() const = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual int32_t GetY() const = 0;

  CS_FUNCTION()
  CS_NODISCARD virtual int32_t GetDeltaX() const = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual int32_t GetDeltaY() const = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual int32_t GetWheel() const = 0;
  CS_FUNCTION()
  CS_NODISCARD virtual int32_t GetWheelHorizontal() const = 0;

  CS_FUNCTION(jenum="org.cryo.core.input.EMouseButton")
  CS_NODISCARD virtual bool IsButtonDown(cs::eMouseButton mouseButton) const = 0;
  CS_FUNCTION(jenum="org.cryo.core.input.EMouseButton")
  CS_NODISCARD virtual bool IsButtonUp(cs::eMouseButton mouseButton) const = 0;
  CS_FUNCTION(jenum="org.cryo.core.input.EMouseButton")
  CS_NODISCARD virtual bool IsButtonPressed(cs::eMouseButton mouseButton) const = 0;
  CS_FUNCTION(jenum="org.cryo.core.input.EMouseButton")
  CS_NODISCARD virtual bool IsButtonReleased(cs::eMouseButton mouseButton) const = 0;
};

}