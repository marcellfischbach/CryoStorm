#pragma once

#include <csCore/input/iInputSystem.hh>
#include <csLauncher/window/csSDLKeyboard.hh>
#include <csLauncher/window/csSDLMouse.hh>
#include <csLauncher/window/csSDLWindow.hh>

namespace cryo::launcher
{

CS_CLASS()
class  csSDLInputSystem : public CS_SUPER(iInputSystem)
{
CS_CLASS_GEN_OBJECT;
public:
  csSDLInputSystem(csSDLKeyboard *keyboard, csSDLMouse *mouse);
  ~csSDLInputSystem() override = default;

  CS_NODISCARD virtual iKeyboard *GetKeyboard() const;
  CS_NODISCARD virtual iMouse *GetMouse() const;


private:
  csSDLKeyboard *m_keyboard;
  csSDLMouse    *m_mouse;
};

}