#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/input/key.hh>
#include <spcCore/input/button.hh>
#include <spcCore/math/vector2f.hh>
#include <spcCore/input/ikeyboard.hh>
#include <spcCore/input/imouse.hh>

namespace spc
{

struct iInputSystem;

class SPC_CORE_API Input
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

  static iMouse *GetMouse ();
  static iKeyboard *GetKeyboard ();
private:
  Input() { }
  static iInputSystem* Get();
  static iInputSystem* m_cache;
};


}