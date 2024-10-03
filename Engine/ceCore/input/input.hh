#pragma once

#include "ceCore/coreexport.hh"
#include "key.hh"
#include "button.hh"
#include "ceCore/math/vector2f.hh"
#include "ikeyboard.hh"
#include "imouse.hh"

namespace cryo
{

struct iInputSystem;

class CS_CORE_API Input
{
public:

  static bool IsKeyDown(Key key);
  static bool IsKeyUp(Key key);
  static bool IsKeyPressed(Key key);
  static bool IsKeyReleased(Key key);

  static Vector2f GetMousePosition();
  static Vector2f GetMouseDelta();
  static Vector2f GetWheel();
  static bool IsButtonDown(eMouseButton mouseButton);
  static bool IsButtonUp(eMouseButton mouseButton);
  static bool IsButtonPressed(eMouseButton mouseButton);
  static bool IsButtonReleased(eMouseButton mouseButton);

  static iMouse *GetMouse ();
  static iKeyboard *GetKeyboard ();
private:
  Input() { }
  static iInputSystem* Get();
  static iInputSystem* m_cache;
};


}