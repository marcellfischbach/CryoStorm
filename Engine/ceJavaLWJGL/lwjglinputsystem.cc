#include <ceJavaLWJGL/lwjglinputsystem.hh>


namespace ce::java
{

LwjglInputSystem::LwjglInputSystem(LwjglKeyboard* keyboard, LwjglMouse* mouse)
    : iInputSystem()
    , m_keyboard(keyboard)
    , m_mouse(mouse)
{
  CS_CLASS_GEN_CONSTR;
}


iKeyboard* LwjglInputSystem::GetKeyboard() const
{
  return m_keyboard;
}

iMouse* LwjglInputSystem::GetMouse() const
{
  return m_mouse;
}

}