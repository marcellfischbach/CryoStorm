
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
  bool compat = false;
  for (size_t i=0, in=args.size(); i<in; i++)
  {
    if (args[i] == "--glProfile" && (i+1) < in)
    {
      i++;
      if (args[i] == "compat" || args[i] == "compatibility")
      {
        compat = true;
      }
    }
  }

  csSDLWindow *window = (csSDLWindow*)csObjectRegistry::Get<iWindow>();
  window->Initialize(compat);

  const std::string &iconName = csSettings::Get().Display().GetText("icon");
  if (!iconName.empty())
  {
    window->SetWindowIcon(csResourceLocator(iconName));
  }

  window->Show();
  return true;
}


}