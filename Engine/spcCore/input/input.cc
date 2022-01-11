
#include <spcCore/input/input.hh>
#include <spcCore/input/iinputsystem.hh>
#include <spcCore/objectregistry.hh>

namespace spc
{


iInputSystem* Input::m_cache = nullptr;

iInputSystem* Input::Get()
{
  if (!m_cache)
  {
    m_cache = ObjectRegistry::Get<iInputSystem>();
  }
  return m_cache;
}


bool Input::IsKeyDown(Key key)
{
  return Get()->GetKeyboard()->IsKeyDown(key);
}

bool Input::IsKeyUp(Key key)
{
  return Get()->GetKeyboard()->IsKeyUp(key);
}

bool Input::IsKeyPressed(Key key)
{
  return Get()->GetKeyboard()->IsKeyPressed(key);
}

bool Input::IsKeyReleased(Key key)
{
  return Get()->GetKeyboard()->IsKeyReleased(key);
}

Vector2f Input::GetMousePosition()
{
  const iMouse* mouse = Get()->GetMouse();
  return Vector2f((float)mouse->GetX(), (float)mouse->GetY());
}

Vector2f Input::GetMouseDelta()
{
  const iMouse* mouse = Get()->GetMouse();
  return Vector2f((float)mouse->GetDeltaX(), (float)mouse->GetDeltaY());
}

Vector2f Input::GetWheel()
{
  const iMouse* mouse = Get()->GetMouse();
  return Vector2f((float)mouse->GetWheelHorizontal(), (float)mouse->GetWheel());
}


bool Input::IsButtonDown(MouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonDown(mouseButton);
}

bool Input::IsButtonUp(MouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonUp(mouseButton);
}

bool Input::IsButtonPressed(MouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonPressed(mouseButton);
}

bool Input::IsButtonReleased(MouseButton mouseButton)
{
  return Get()->GetMouse()->IsButtonReleased(mouseButton);
}

iMouse *Input::GetMouse()
{
  return Get()->GetMouse();
}

iKeyboard *Input::GetKeyboard()
{
  return Get()->GetKeyboard();
}


}