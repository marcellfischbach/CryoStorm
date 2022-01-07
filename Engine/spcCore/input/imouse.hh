
#pragma once

#include <spcCore/defs.hh>
#include <spcCore/types.hh>
#include <spcCore/input/button.hh>
#include <spcCore/input/ecursormode.hh>

namespace spc
{


struct iMouse
{
  virtual ~iMouse() = default;

  virtual void SetVisible (bool visible) = 0;
  SPC_NODISCARD virtual bool IsVisible () const = 0;

  virtual void SetCursorMode (eCursorMode mode) = 0;
  SPC_NODISCARD virtual eCursorMode GetCursorMode () const = 0;


  SPC_NODISCARD virtual int32_t GetX() const = 0;
  SPC_NODISCARD virtual int32_t GetY() const = 0;

  SPC_NODISCARD virtual int32_t GetDeltaX() const = 0;
  SPC_NODISCARD virtual int32_t GetDeltaY() const = 0;
  SPC_NODISCARD virtual int32_t GetWheel() const = 0;
  SPC_NODISCARD virtual int32_t GetWheelHorizontal() const = 0;

  SPC_NODISCARD virtual bool IsButtonDown(MouseButton mouseButton) const = 0;
  SPC_NODISCARD virtual bool IsButtonUp(MouseButton mouseButton) const = 0;
  SPC_NODISCARD virtual bool IsButtonPressed(MouseButton mouseButton) const = 0;
  SPC_NODISCARD virtual bool IsButtonReleased(MouseButton mouseButton) const = 0;
};

}