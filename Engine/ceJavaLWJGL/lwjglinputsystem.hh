#pragma once

#include <ceCore/input/iInputSystem.hh>
#include <ceJavaLWJGL/lwjglkeyboard.hh>
#include <ceJavaLWJGL/lwjglmouse.hh>
#include <ceJavaLWJGL/lwjglwindow.hh>

namespace cryo::java
{

CS_CLASS(jclass="org.crimsonedge.lwjgl.LwjglInputSystem")
class  LwjglInputSystem : public CS_SUPER(iInputSystem)
{
CS_CLASS_GEN_OBJECT;
public:
  LwjglInputSystem(LwjglKeyboard *keyboard, LwjglMouse *mouse);
  ~LwjglInputSystem() override = default;

  CS_NODISCARD virtual iKeyboard *GetKeyboard() const;
  CS_NODISCARD virtual iMouse *GetMouse() const;


private:
  LwjglKeyboard *m_keyboard;
  LwjglMouse    *m_mouse;
};

}