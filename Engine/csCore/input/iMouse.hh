
#pragma once

#include <csCore/csDefs.hh>
#include <csCore/csTypes.hh>
#include <csCore/input/eMouseButton.hh>
#include <csCore/input/eCursorMode.hh>

namespace cryo
{


struct iMouse
{
  virtual ~iMouse() = default;

  virtual void SetVisible (bool visible) = 0;
  CS_NODISCARD virtual bool IsVisible () const = 0;

  virtual void SetCursorMode (eCursorMode mode) = 0;
  CS_NODISCARD virtual eCursorMode GetCursorMode () const = 0;


  CS_NODISCARD virtual int32_t GetX() const = 0;
  CS_NODISCARD virtual int32_t GetY() const = 0;

  CS_NODISCARD virtual int32_t GetDeltaX() const = 0;
  CS_NODISCARD virtual int32_t GetDeltaY() const = 0;
  CS_NODISCARD virtual int32_t GetWheel() const = 0;
  CS_NODISCARD virtual int32_t GetWheelHorizontal() const = 0;

  CS_NODISCARD virtual bool IsButtonDown(eMouseButton mouseButton) const = 0;
  CS_NODISCARD virtual bool IsButtonUp(eMouseButton mouseButton) const = 0;
  CS_NODISCARD virtual bool IsButtonPressed(eMouseButton mouseButton) const = 0;
  CS_NODISCARD virtual bool IsButtonReleased(eMouseButton mouseButton) const = 0;
};

}