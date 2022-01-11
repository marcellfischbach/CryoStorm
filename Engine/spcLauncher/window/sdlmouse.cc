
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
  , m_cursorVisible(true)
  , m_cursorMode(eCursorMode::Free)
  , m_window(nullptr)
{
  for (int i = 0; i < eMB_COUNT; i++)
  {
    m_current[i] = m_last[i] = false;
  }
}

void SDLMouse::SetWindow(SDL_Window *window)
{
  m_window = window;
}

void SDLMouse::SetVisible(bool visible)
{
  SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
  m_cursorVisible = SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
}

bool SDLMouse::IsVisible() const
{
  return m_cursorVisible;
}

void SDLMouse::SetCursorMode(eCursorMode mode)
{
  m_cursorMode = mode;
  switch (m_cursorMode)
  {
  case eCursorMode::Free:
    SDL_SetWindowGrab(m_window, SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    break;
  case eCursorMode::Confined:
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_SetWindowGrab(m_window, SDL_TRUE);
    break;
  case eCursorMode::Fixed:
    SDL_SetWindowGrab(m_window, SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    break;
  }
  SetVisible(m_cursorVisible);
}

eCursorMode SDLMouse::GetCursorMode() const
{
  return m_cursorMode;
}

int32_t SDLMouse::GetX() const
{
  return m_x;
}

int32_t SDLMouse::GetY() const
{
  return m_y;
}

int32_t SDLMouse::GetDeltaX() const
{
  return m_relX;
}

int32_t SDLMouse::GetDeltaY() const
{
  return m_relY;
}

int32_t SDLMouse::GetWheel() const
{
  return m_wheel;
}

int32_t SDLMouse::GetWheelHorizontal() const
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
  m_relX = 0;
  m_relY = 0;
  memcpy(m_last, m_current, sizeof(bool) * eMB_COUNT);

  int width, height;
//  SDL_GetWindowSize(m_window, &width, &height);
//  SDL_WarpMouseInWindow(m_window, width / 2, height / 2);
}

void SDLMouse::Update(uint8_t button, bool down)
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

void SDLMouse::Update(int32_t x, int32_t y, int32_t relX, int32_t relY)
{
  m_x = x;
  m_y = y;
  m_relX = relX;
  m_relY = relY;
}

}