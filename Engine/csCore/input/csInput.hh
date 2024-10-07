#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/input/eKey.hh>
#include <csCore/input/eMouseButton.hh>
#include <csCore/math/csVector2f.hh>
#include <csCore/input/iKeyboard.hh>
#include <csCore/input/iMouse.hh>

namespace cs
{

struct iInputSystem;

class CS_CORE_API csInput
{
public:

  static bool IsKeyDown(eKey key);
  static bool IsKeyUp(eKey key);
  static bool IsKeyPressed(eKey key);
  static bool IsKeyReleased(eKey key);

  static csVector2f GetMousePosition();
  static csVector2f GetMouseDelta();
  static csVector2f GetWheel();
  static bool IsButtonDown(eMouseButton mouseButton);
  static bool IsButtonUp(eMouseButton mouseButton);
  static bool IsButtonPressed(eMouseButton mouseButton);
  static bool IsButtonReleased(eMouseButton mouseButton);

  static iMouse *GetMouse ();
  static iKeyboard *GetKeyboard ();
private:
  csInput() { }
  static iInputSystem* Get();
  static iInputSystem* m_cache;
};


}