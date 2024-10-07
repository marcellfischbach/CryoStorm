
#include <csCore/input/csInput.hh>
#include <csCore/input/iInputSystem.hh>
#include <csCore/csObjectRegistry.hh>

namespace cs
{


iInputSystem* csInput::m_cache = nullptr;

iInputSystem* csInput::Get()
{
  if (!m_cache)
  {
    m_cache = csObjectRegistry::Get<iInputSystem>();
  }
  return m_cache;
}


bool csInput::IsKeyDown(eKey key)
{
  return Get()->GetKeyboard()->IsKeyDown(key);
}

bool csInput::IsKeyUp(eKey key)
{
  return Get()->GetKeyboard()->IsKeyUp(key);
}

bool csInput::IsKeyPressed(eKey key)
{
  return Get()->GetKeyboard()->IsKeyPressed(key);
}

bool csInput::IsKeyReleased(eKey key)
{
  return Get()->GetKeyboard()->IsKeyReleased(key);
}

csVector2f csInput::GetMousePosition()
{
  const iMouse* mouse = Get()->GetMouse();
  return csVector2f((float)mouse->GetX(), (float)mouse->GetY());
}

csVector2f csInput::GetMouseDelta()
{
  const iMouse* mouse = Get()->GetMouse();
  return csVector2f((float)mouse->GetDeltaX(), (float)mouse->GetDeltaY());
}

csVector2f csInput::GetWheel()
{
  const iMouse* mouse = Get()->GetMouse();
  return csVector2f((float)mouse->GetWheelHorizontal(), (float)mouse->GetWheel());
}


bool csInput::IsButtonDown(eMouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonDown(mouseButton);
}

bool csInput::IsButtonUp(eMouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonUp(mouseButton);
}

bool csInput::IsButtonPressed(eMouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonPressed(mouseButton);
}

bool csInput::IsButtonReleased(eMouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonReleased(mouseButton);
}

iMouse *csInput::GetMouse()
{
  return Get()->GetMouse();
}

iKeyboard *csInput::GetKeyboard()
{
  return Get()->GetKeyboard();
}


}