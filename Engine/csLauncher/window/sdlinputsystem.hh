#pragma once

#include <csCore/input/iInputSystem.hh>
#include <csLauncher/window/sdlkeyboard.hh>
#include <csLauncher/window/sdlmouse.hh>
#include <csLauncher/window/sdlwindow.hh>

namespace cryo::launcher
{

CS_CLASS()
class  SDLInputSystem : public CS_SUPER(iInputSystem)
{
CS_CLASS_GEN_OBJECT;
public:
  SDLInputSystem(SDLKeyboard *keyboard, SDLMouse *mouse);
  ~SDLInputSystem() override = default;

  CS_NODISCARD virtual iKeyboard *GetKeyboard() const;
  CS_NODISCARD virtual iMouse *GetMouse() const;


private:
  SDLKeyboard *m_keyboard;
  SDLMouse    *m_mouse;
};

}