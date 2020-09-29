
#pragma once

#include <SpiceCore/types.hh>
#include <SpiceCore/input/button.hh>

namespace Spice
{


struct iMouse
{
  virtual ~iMouse() {}

  virtual Int16 GetX() const = 0;
  virtual Int16 GetY() const = 0;
  virtual Int16 GetDeltaX() const = 0;
  virtual Int16 GetDeltaY() const = 0;
  virtual Int16 GetWheel() const = 0;
  virtual Int16 GetWheelHorizontal() const = 0;

  virtual bool IsButtonDown(MouseButton mouseButton) const = 0;
  virtual bool IsButtonUp(MouseButton mouseButton) const = 0;
  virtual bool IsButtonPressed(MouseButton mouseButton) const = 0;
  virtual bool IsButtonReleased(MouseButton mouseButton) const = 0;
};

}