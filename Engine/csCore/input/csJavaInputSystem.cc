//
// Created by MCEL on 21.03.2025.
//

#include <csCore/input/csJavaInputSystem.hh>
#include <csCore/input/csJavaKeyboard.hh>
#include <csCore/input/csJavaMouse.hh>

namespace cs
{

void csJavaInputSystem::SetKeyboard(cs::csJavaKeyboard *keyboard)
{
  m_keyboard = keyboard;
}

void csJavaInputSystem::SetMouse(cs::csJavaMouse *mouse)
{
  m_mouse = mouse;
}

iKeyboard *csJavaInputSystem::GetKeyboard() const
{
  return m_keyboard;
}
iMouse *csJavaInputSystem::GetMouse() const
{
  return m_mouse;
}
} // cs