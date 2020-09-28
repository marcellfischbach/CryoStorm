
#include <cesdlwindow/cesdlwindowmodule.hh>

#include <SDL.h>


bool ceSDLWindowModule::Initialize(int argc, char** argv)
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
  printf ("ceSDLWindowModule::Initialize\n");
  return true;
}


