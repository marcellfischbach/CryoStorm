
#include <SpiceSDLWindow/sdlwindow.hh>

#include <SDL.h>

namespace Spice
{

SDLWindow::SDLWindow()
  : iWindow()
  , m_window(nullptr)
{
  SPICE_CLASS_GEN_CONSTR;
}



void SDLWindow::SetTitle(const std::string& title)
{
  m_title = title;
  if (m_window)
  {
    SDL_SetWindowTitle(m_window, title.c_str());
  }
}

void SDLWindow::SetPosition (Int16 x, Int16 y)
{
  m_x = x;
  m_y = y;
  if (m_window)
  {
    SDL_SetWindowPosition(m_window, x, y);
  }
}

void SDLWindow::SetResolution(UInt16 width, UInt16 height)
{
  m_width = width;
  m_height = height;
  if (m_window)
  {
    SDL_SetWindowSize(m_window, width, height);
  }
}

void SDLWindow::Show ()
{
  Create();
  SDL_ShowWindow(m_window);
}

void SDLWindow::Present ()
{
  SDL_GL_SwapWindow(m_window);
}

void SDLWindow::ProcessUpdates()
{
  SDL_Event evt;
  while (SDL_PollEvent(&evt))
  {
  }
}

void SDLWindow::Create()
{
  if (m_window)
  {
    return;
  }
  m_window = SDL_CreateWindow(m_title.c_str(), m_x, m_y, m_width, m_height, SDL_WINDOW_OPENGL);
}

}
