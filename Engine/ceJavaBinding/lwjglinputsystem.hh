#pragma once

#include <ceCore/input/iinputsystem.hh>
#include <ceJavaBinding/lwjglkeyboard.hh>
#include <ceJavaBinding/lwjglmouse.hh>
#include <ceJavaBinding/lwjglwindow.hh>

namespace ce::java
{

CE_CLASS(jclass="org.crimsonedge.launcher.LwjglInputSystem")
class  LwjglInputSystem : public CE_SUPER(iInputSystem)
{
CE_CLASS_GEN_OBJECT;
public:
  LwjglInputSystem(LwjglKeyboard *keyboard, LwjglMouse *mouse);
  ~LwjglInputSystem() override = default;

  CE_NODISCARD virtual iKeyboard *GetKeyboard() const;
  CE_NODISCARD virtual iMouse *GetMouse() const;


private:
  LwjglKeyboard *m_keyboard;
  LwjglMouse    *m_mouse;
};

}