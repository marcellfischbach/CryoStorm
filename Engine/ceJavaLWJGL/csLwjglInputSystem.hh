#pragma once

#include <ceCore/input/iInputSystem.hh>
#include <ceJavaLWJGL/csLwjglKeyboard.hh>
#include <ceJavaLWJGL/csLwjglMouse.hh>
#include <ceJavaLWJGL/csLwjglWindow.hh>

namespace cryo::java
{

CS_CLASS(jclass="org.crimsonedge.lwjgl.LwjglInputSystem")
class  csLwjglInputSystem : public CS_SUPER(iInputSystem)
{
CS_CLASS_GEN_OBJECT;
public:
  csLwjglInputSystem(csLwjglKeyboard *keyboard, csLwjglMouse *mouse);
  ~csLwjglInputSystem() override = default;

  CS_NODISCARD virtual iKeyboard *GetKeyboard() const;
  CS_NODISCARD virtual iMouse *GetMouse() const;


private:
  csLwjglKeyboard *m_keyboard;
  csLwjglMouse    *m_mouse;
};

}