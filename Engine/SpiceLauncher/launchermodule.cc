
#include <SpiceLauncher/launchermodule.hh>
#include <SpiceLauncher/master.refl.cc>
#include <SpiceLauncher/window/sdlkeyboard.hh>
#include <SpiceLauncher/window/sdlmouse.hh>
#include <SpiceCore/objectregistry.hh>

extern Spice::SDLKeyboard keyboard;
extern Spice::SDLMouse mouse;

namespace Spice
{

bool LauncherModule::Register(int argc, char** argv)
{
  register_classes();

  Spice::SDLInputSystem* inputSystem = new Spice::SDLInputSystem(&keyboard, &mouse);
  Spice::ObjectRegistry::Register<Spice::iInputSystem>(inputSystem);

  return true;
}

bool LauncherModule::Initialize(int argc, char** argv)
{
  return true;
}

}