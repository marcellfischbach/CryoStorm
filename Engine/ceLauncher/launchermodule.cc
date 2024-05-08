
#include <ceLauncher/launchermodule.hh>
#include <ceLauncher/window/sdlwindow.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/engine.hh>
#include <ceCore/settings.hh>

#include <master.refl.cc>

namespace ce::launcher
{

bool LauncherModule::Register(const std::vector<std::string> &args, Engine *engine)
{
  register_classes();

  SDLWindow *window = new SDLWindow();
  SDLInputSystem *inputSystem = new SDLInputSystem(window->GetKeyboard(), window->GetMouse());
  ObjectRegistry::Register<iInputSystem>(inputSystem);
  ObjectRegistry::Register<iWindow>(window);
  engine->SetWindow(window);

  return true;
}

bool LauncherModule::Initialize(const std::vector<std::string> &args, Engine *engine)
{
  SDLWindow *window = (SDLWindow*)engine->GetWindow();
  window->Initialize();

  const std::string &iconName = Settings::Get().Display().GetText("icon");
  if (!iconName.empty())
  {
    window->SetWindowIcon(ResourceLocator(iconName));
  }

  window->Show();
  return true;
}


}