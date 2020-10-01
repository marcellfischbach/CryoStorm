
#include <spcLauncher/launchermodule.hh>
#include <spcLauncher/master.refl.cc>
#include <spcLauncher/window/sdlkeyboard.hh>
#include <spcLauncher/window/sdlmouse.hh>
#include <spcCore/objectregistry.hh>

extern spc::SDLKeyboard keyboard;
extern spc::SDLMouse mouse;

namespace spc
{

bool LauncherModule::Register(int argc, char** argv)
{
  register_classes();

  spc::SDLInputSystem* inputSystem = new spc::SDLInputSystem(&keyboard, &mouse);
  spc::ObjectRegistry::Register<spc::iInputSystem>(inputSystem);

  return true;
}

bool LauncherModule::Initialize(int argc, char** argv)
{
  return true;
}

}