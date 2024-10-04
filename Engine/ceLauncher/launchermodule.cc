
#include <ceLauncher/launchermodule.hh>
#include <ceLauncher/window/sdlwindow.hh>
#include <ceCore/csObjectRegistry.hh>
#include <ceCore/csEngine.hh>
#include <ceCore/csSettings.hh>

#include <master.refl.cc>

namespace cryo::launcher
{

bool LauncherModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

  SDLWindow *window = new SDLWindow();
  SDLInputSystem *inputSystem = new SDLInputSystem(window->GetKeyboard(), window->GetMouse());
  csObjectRegistry::Register<iInputSystem>(inputSystem);
  csObjectRegistry::Register<iWindow>(window);
  engine->SetWindow(window);

  return true;
}

bool LauncherModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  SDLWindow *window = (SDLWindow*)engine->GetWindow();
  window->Initialize();

  const std::string &iconName = csSettings::Get().Display().GetText("icon");
  if (!iconName.empty())
  {
    window->SetWindowIcon(csResourceLocator(iconName));
  }

  window->Show();
  return true;
}


}