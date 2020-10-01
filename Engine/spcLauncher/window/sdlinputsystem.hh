#pragma once

#include <spcCore/input/iinputsystem.hh>
#include <spcLauncher/window/sdlkeyboard.hh>
#include <spcLauncher/window/sdlmouse.hh>

namespace spc
{

SPC_CLASS()
class SDLInputSystem : public SPC_SUPER(iInputSystem)
{
  SPC_CLASS_GEN_OBJECT;
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