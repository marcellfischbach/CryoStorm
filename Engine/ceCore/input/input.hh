#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/input/key.hh>
#include <ceCore/input/button.hh>
#include <ceCore/math/vector2f.hh>
#include <ceCore/input/ikeyboard.hh>
#include <ceCore/input/imouse.hh>

namespace ce
{

struct iInputSystem;

class CE_CORE_API Input
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