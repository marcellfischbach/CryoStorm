
#include <SpiceSDLWindow/sdlwindowmodule.hh>
#include <SpiceSDLWindow/sdlwindow.hh>
#include <SpiceSDLWindow/master.refl.cc>
#include <SpiceCore/objectregistry.hh>

#include <SDL.h>

namespace Spice
{

bool SDLWindowModule::Initialize(int argc, char** argv)
{
  register_classes();

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);


  SDLWindow* wnd = new SDLWindow();
  ObjectRegistry::Get()->Register(iWindow::GetStaticClass(), wnd);
  return true;
}



}
