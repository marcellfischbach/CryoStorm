#pragma once

#include <SpiceCore/input/iinputsystem.hh>
#include <SpiceLauncher/window/sdlkeyboard.hh>
#include <SpiceLauncher/window/sdlmouse.hh>

namespace Spice
{

SPICE_CLASS()
class SDLInputSystem : public SPICE_SUPER(iInputSystem)
{
  SPICE_CLASS_GEN_OBJECT;
public:
  SDLInputSystem(SDLKeyboard * keyboard, SDLMouse * mouse);

  virtual ~SDLInputSystem()
  {

  }

  virtual const iKeyboard* GetKeyboard() const;
  virtual const iMouse* GetMouse() const;


private:
  SDLKeyboard* m_keyboard;
  SDLMouse* m_mouse;

};

}