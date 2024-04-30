#pragma once

#include "ceCore/input/iinputsystem.hh"
#include <ceSDLWindow/sdlkeyboard.hh>
#include <ceSDLWindow/sdlmouse.hh>
#include <ceSDLWindow/sdlwindow.hh>

namespace ce::sdlwindow
{

CE_CLASS()
class CE_SDL_API SDLInputSystem : public CE_SUPER(iInputSystem)
{
CE_CLASS_GEN_OBJECT;
public:
  SDLInputSystem(SDLKeyboard *keyboard, SDLMouse *mouse);
  ~SDLInputSystem() override = default;

  CE_NODISCARD virtual iKeyboard *GetKeyboard() const;
  CE_NODISCARD virtual iMouse *GetMouse() const;


private:
  SDLKeyboard *m_keyboard;
  SDLMouse    *m_mouse;
};

}