
#pragma once

#include <SpiceSDLWindow/sdlexport.hh>
#include <SpiceCore/window/iwindow.hh>
#include <SpiceSDLWindow/sdlwindow.refl.hh>

struct SDL_Window;

namespace Spice
{

SPICE_CLASS()
class SPICE_SDL_API SDLWindow : public SPICE_SUPER(Spice::iWindow)
{
  SPICE_CLASS_GEN_OBJECT;
public:
  SDLWindow();
  virtual ~SDLWindow() { }

  virtual void SetTitle(const std::string& title);

  virtual void SetPosition (Int16 x, Int16 y);

  virtual void SetResolution(UInt16 width, UInt16 height);

  virtual void Show ();

  virtual void Present ();

  virtual void ProcessUpdates();

private:
    void Create ();

    std::string m_title;
    Int16 m_x;
    Int16 m_y;
    UInt16 m_width;
    UInt16 m_height;

    SDL_Window *m_window;

};


}
