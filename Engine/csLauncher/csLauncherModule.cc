
#include <csLauncher/csLauncherModule.hh>
#include <csLauncher/window/csSDLWindow.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/csEngine.hh>
#include <csCore/csSettings.hh>

#include <master.refl.cc>

namespace cs::launcher
{

bool csLauncherModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

  csSDLWindow      *window      = new csSDLWindow();
  csSDLInputSystem *inputSystem = new csSDLInputSystem(window->GetKeyboard(), window->GetMouse());
  csObjectRegistry::Register<iInputSystem>(inputSystem);
  csObjectRegistry::Register<iWindow>(window);

  return true;
}

bool csLauncherModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  csSDLWindow *window = (csSDLWindow*)csObjectRegistry::Get<iWindow>();
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