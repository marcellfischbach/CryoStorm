
#include <csLauncher/csLauncherModule.hh>
#include <csLauncher/window/csSDLWindow.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/csEngine.hh>
#include <csCore/csSettings.hh>

#include <master.refl.cc>

namespace cryo::launcher
{

bool csLauncherModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

  csSDLWindow      *window      = new csSDLWindow();
  csSDLInputSystem *inputSystem = new csSDLInputSystem(window->GetKeyboard(), window->GetMouse());
  csObjectRegistry::Register<iInputSystem>(inputSystem);
  csObjectRegistry::Register<iWindow>(window);
  engine->SetWindow(window);

  return true;
}

bool csLauncherModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  csSDLWindow *window = (csSDLWindow*)engine->GetWindow();
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