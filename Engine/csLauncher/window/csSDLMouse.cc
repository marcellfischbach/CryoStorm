
#include <csLauncher/window/csSDLMouse.hh>
#include <csLauncher/window/csSDLButtonMap.hh>

namespace cs::launcher
{

csSDLMouse::csSDLMouse()
  : m_x(0)
  , m_y(0)
  , m_relX(0)
  , m_relY(0)
  , m_wheel(0)
  , m_wheelHorizontal(0)
  , m_cursorVisible(true)
  , m_cursorMode(eCursorMode::eCM_Free)
  , m_window(nullptr)
{
  for (int i = 0; i < (size_t)eMouseButton::eMB_COUNT; i++)
  {
    m_current[i] = m_last[i] = false;
  }
}

void csSDLMouse::SetWindow(SDL_Window *window)
{
  m_window = window;
}

void csSDLMouse::SetVisible(bool visible)
{
  SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
  m_cursorVisible = SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
}

bool csSDLMouse::IsVisible() const
{
  return m_cursorVisible;
}

void csSDLMouse::SetCursorMode(eCursorMode mode)
{
  m_cursorMode = mode;
  switch (m_cursorMode)
  {
  case eCursorMode::eCM_Free:
    SDL_SetWindowGrab(m_window, SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    break;
  case eCursorMode::eCM_Confined:
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_SetWindowGrab(m_window, SDL_TRUE);
    break;
  case eCursorMode::eCM_Fixed:
    SDL_SetWindowGrab(m_window, SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    break;
  }
  SetVisible(m_cursorVisible);
}

eCursorMode csSDLMouse::GetCursorMode() const
{
  return m_cursorMode;
}

int32_t csSDLMouse::GetX() const
{
  return m_x;
}

int32_t csSDLMouse::GetY() const
{
  return m_y;
}

int32_t csSDLMouse::GetDeltaX() const
{
  return m_relX;
}

int32_t csSDLMouse::GetDeltaY() const
{
  return m_relY;
}

int32_t csSDLMouse::GetWheel() const
{
  return m_wheel;
}

int32_t csSDLMouse::GetWheelHorizontal() const
{
  return m_wheelHorizontal;
}


bool csSDLMouse::IsButtonDown(eMouseButton mouseButton) const
{
  return m_current[(size_t)mouseButton];
}

bool csSDLMouse::IsButtonUp(eMouseButton mouseButton) const
{
  return !m_current[(size_t)mouseButton];
}

bool csSDLMouse::IsButtonPressed(eMouseButton mouseButton) const
{
  return !m_last[(size_t)mouseButton] && m_current[(size_t)mouseButton];
}

bool csSDLMouse::IsButtonReleased(eMouseButton mouseButton) const
{
  return m_last[(size_t)mouseButton] && !m_current[(size_t)mouseButton];
}


void csSDLMouse::Update()
{
  m_wheel = 0;
  m_wheelHorizontal = 0;
  m_relX = 0;
  m_relY = 0;
  memcpy(m_last, m_current, sizeof(bool) * (size_t)eMouseButton::eMB_COUNT);

}

void csSDLMouse::Update(uint8_t button, bool down)
{
  if (button < (size_t)eMouseButton::eMB_COUNT)
  {
    m_current[(size_t)Map(button)] = down;
  }
}

void csSDLMouse::Update(int32_t wheel, int32_t wheelHorizontal)
{
  m_wheel = wheel;
  m_wheelHorizontal = wheelHorizontal;
}

void csSDLMouse::Update(int32_t x, int32_t y, int32_t relX, int32_t relY)
{
  m_x = x;
  m_y = y;
  m_relX = relX;
  m_relY = relY;
}

}