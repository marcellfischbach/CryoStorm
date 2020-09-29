#pragma once

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/input/key.hh>
#include <SpiceCore/input/button.hh>
#include <SpiceCore/math/vector2f.hh>

namespace Spice
{

struct iInputSystem;

class SPICE_CORE_API Input
{
public:

  static bool IsKeyDown(Key key);
  static bool IsKeyUp(Key key);
  static bool IsKeyPressed(Key key);
  static bool IsKeyReleased(Key key);
  


  static Vector2f GetMousePosition();
  static Vector2f GetMouseDelta();
  static Vector2f GetWheel();
  static bool IsButtonDown(MouseButton mouseButton);
  static bool IsButtonUp(MouseButton mouseButton);
  static bool IsButtonPressed(MouseButton mouseButton);
  static bool IsButtonReleased(MouseButton mouseButton);

private:
  Input() { }
  static iInputSystem* Get();
  static iInputSystem* m_cache;
};


}