//
// Created by MCEL on 08.05.2024.
//

#include <ceJavaBinding/lwjglmouse.hh>


namespace ce::java
{

void LwjglMouse::SetVisible(bool visible)
{
}

bool LwjglMouse::IsVisible() const
{
  return true;
}

void LwjglMouse::SetCursorMode(eCursorMode mode)
{

}

eCursorMode LwjglMouse::GetCursorMode() const
{
  return eCursorMode::Free;
}


int32_t LwjglMouse::GetX() const
{
  return 0;
}

int32_t LwjglMouse::GetY() const
{
  return 0;
}

int32_t LwjglMouse::GetDeltaX() const
{
  return 0;
}

int32_t LwjglMouse::GetDeltaY() const
{
  return 0;
}

int32_t LwjglMouse::GetWheel() const
{
  return 0;
}
int32_t LwjglMouse::GetWheelHorizontal() const
{
  return 0;
}

bool LwjglMouse::IsButtonDown(eMouseButton mouseButton) const
{
  return false;
}
bool LwjglMouse::IsButtonUp(eMouseButton mouseButton) const
{
  return true;
}
bool LwjglMouse::IsButtonPressed(eMouseButton mouseButton) const
{
  return false;
}
bool LwjglMouse::IsButtonReleased(eMouseButton mouseButton) const
{
  return false;
}

}