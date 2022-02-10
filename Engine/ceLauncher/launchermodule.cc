
#include <ceLauncher/launchermodule.hh>
#include <master.refl.cc>
#include <ceLauncher/window/sdlkeyboard.hh>
#include <ceLauncher/window/sdlmouse.hh>
#include <ceCore/objectregistry.hh>

extern ce::SDLKeyboard keyboard;
extern ce::SDLMouse mouse;

namespace ce
{

bool LauncherModule::Register(int argc, char** argv)
{
  register_classes();

  ce::SDLInputSystem* inputSystem = new ce::SDLInputSystem(&keyboard, &mouse);
  ce::ObjectRegistry::Register<ce::iInputSystem>(inputSystem);

  return true;
}

bool LauncherModule::Initialize(int argc, char** argv)
{
  return true;
}

}