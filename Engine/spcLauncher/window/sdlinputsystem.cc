#include <spcLauncher/window/sdlinputsystem.hh>


namespace spc
{

SDLInputSystem::SDLInputSystem(SDLKeyboard* keyboard, SDLMouse* mouse)
  : iInputSystem()
  , m_keyboard(keyboard)
  , m_mouse(mouse)
{
  SPC_CLASS_GEN_CONSTR;
}


iKeyboard* SDLInputSystem::GetKeyboard() const
{
  return m_keyboard;
}

iMouse* SDLInputSystem::GetMouse() const
{
  return m_mouse;
}

}