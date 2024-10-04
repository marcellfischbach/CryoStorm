#pragma once

#include "ceCore/csCoreExport.hh"
#include "eKey.hh"
#include "eMouseButton.hh"
#include "ceCore/math/csVector2f.hh"
#include "iKeyboard.hh"
#include "iMouse.hh"

namespace cryo
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