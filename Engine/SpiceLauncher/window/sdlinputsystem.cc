#include <SpiceLauncher/window/sdlinputsystem.hh>


namespace Spice
{

SDLInputSystem::SDLInputSystem(SDLKeyboard* keyboard, SDLMouse* mouse)
  : iInputSystem()
  , m_keyboard(keyboard)
  , m_mouse(mouse)
{
  SPICE_CLASS_GEN_CONSTR;
}


const iKeyboard* SDLInputSystem::GetKeyboard() const
{
  return m_keyboard;
}

const iMouse* SDLInputSystem::GetMouse() const
{
  return m_mouse;
}

}