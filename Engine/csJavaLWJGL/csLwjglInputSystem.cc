#include <csJavaLWJGL/csLwjglInputSystem.hh>


namespace cs::java
{

csLwjglInputSystem::csLwjglInputSystem(csLwjglKeyboard* keyboard, csLwjglMouse* mouse)
    : iInputSystem()
    , m_keyboard(keyboard)
    , m_mouse(mouse)
{
  CS_CLASS_GEN_CONSTR;
}


iKeyboard* csLwjglInputSystem::GetKeyboard() const
{
  return m_keyboard;
}

iMouse* csLwjglInputSystem::GetMouse() const
{
  return m_mouse;
}

}