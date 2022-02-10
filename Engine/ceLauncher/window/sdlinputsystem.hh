#pragma once

#include <ceCore/input/iinputsystem.hh>
#include <ceLauncher/window/sdlkeyboard.hh>
#include <ceLauncher/window/sdlmouse.hh>

namespace ce
{

CE_CLASS()
class SDLInputSystem : public CE_SUPER(iInputSystem)
{
  CE_CLASS_GEN_OBJECT;
public:
  SDLInputSystem(SDLKeyboard * keyboard, SDLMouse * mouse);
  ~SDLInputSystem() override = default;

  CE_NODISCARD virtual iKeyboard* GetKeyboard() const;
  CE_NODISCARD virtual iMouse* GetMouse() const;


private:
  SDLKeyboard* m_keyboard;
  SDLMouse* m_mouse;

};

}