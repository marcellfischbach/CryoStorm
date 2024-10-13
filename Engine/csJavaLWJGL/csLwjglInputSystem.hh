#pragma once

#include <csCore/input/iInputSystem.hh>
#include <csJavaLWJGL/csLwjglKeyboard.hh>
#include <csJavaLWJGL/csLwjglMouse.hh>
#include <csJavaLWJGL/csLwjglWindow.hh>

namespace cs::java
{

CS_CLASS(jclass="org.cryo.lwjgl.LwjglInputSystem")
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