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
  ~SDLInputSystem() override = default;

  SPC_NODISCARD virtual iKeyboard* GetKeyboard() const;
  SPC_NODISCARD virtual iMouse* GetMouse() const;


private:
  SDLKeyboard* m_keyboard;
  SDLMouse* m_mouse;

};

}