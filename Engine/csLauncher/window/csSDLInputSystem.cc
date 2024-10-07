#include <csLauncher/window/csSDLInputSystem.hh>


namespace cs::launcher
{

csSDLInputSystem::csSDLInputSystem(csSDLKeyboard* keyboard, csSDLMouse* mouse)
  : iInputSystem()
  , m_keyboard(keyboard)
  , m_mouse(mouse)
{
  CS_CLASS_GEN_CONSTR;
}


iKeyboard* csSDLInputSystem::GetKeyboard() const
{
  return m_keyboard;
}

iMouse* csSDLInputSystem::GetMouse() const
{
  return m_mouse;
}

}