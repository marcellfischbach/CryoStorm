
#include <spcLauncher/window/sdlmouse.hh>
#include <spcLauncher/window/sdlbutton_map.hh>

namespace spc
{

SDLMouse::SDLMouse()
  : m_x(0)
  , m_y(0)
  , m_relX(0)
  , m_relY(0)
  , m_wheel(0)
  , m_wheelHorizontal(0)
{
  for (int i = 0; i < eMB_COUNT; i++)
  {
    m_current[i] = m_last[i] = false;
  }
}

Int16 SDLMouse::GetX() const
{
  return m_x;
}

Int16 SDLMouse::GetY() const
{
  return m_y;
}

Int16 SDLMouse::GetDeltaX() const
{
  return m_relX;
}

Int16 SDLMouse::GetDeltaY() const
{
  return m_relY;
}

Int16 SDLMouse::GetWheel() const
{
  return m_wheel;
}

Int16 SDLMouse::GetWheelHorizontal() const
{
  return m_wheelHorizontal;
}


bool SDLMouse::IsButtonDown(MouseButton mouseButton) const 
{
  return m_current[mouseButton];
}

bool SDLMouse::IsButtonUp(MouseButton mouseButton) const 
{
  return !m_current[mouseButton];
}

bool SDLMouse::IsButtonPressed(MouseButton mouseButton) const 
{
  return !m_last[mouseButton] && m_current[mouseButton];
}

bool SDLMouse::IsButtonReleased(MouseButton mouseButton) const 
{
  return m_last[mouseButton] && !m_current[mouseButton];
}


void SDLMouse::Update()
{
  m_wheel = 0;
  m_wheelHorizontal = 0;
  memcpy(m_last, m_current, sizeof(bool) * eMB_COUNT);
}

void SDLMouse::Update(UInt8 button, bool down)
{
  if (button < eMB_COUNT)
  {
    m_current[Map(button)] = down;
  }
}

void SDLMouse::Update(Int32 wheel, Int32 wheelHorizontal)
{
  m_wheel = wheel;
  m_wheelHorizontal = wheelHorizontal;
}

void SDLMouse::Update(Int16 x, Int16 y, Int16 relX, Int16 relY)
{
  m_x = x;
  m_y = y;
  m_relX = relX;
  m_relY = relY;
}

}