#include <csJavaLWJGL/csLwjglInputSystem.hh>


namespace cs::java
{

csLwjglInputSystem::csLwjglInputSystem(csLwjglKeyboard* keyboard, csLwjglMouse* mouse)
    : iInputSystem()
    , m_keyboard(keyboard)
    , m_mouse(mouse)
{

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