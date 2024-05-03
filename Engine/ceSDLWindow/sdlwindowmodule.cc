//
// Created by mcel on 30.04.2024.
//

#include <ceSDLWindow/sdlwindowmodule.hh>
#include <ceSDLWindow/sdlinputsystem.hh>
#include <ceSDLWindow/sdlwindow.hh>

#include <ceCore/engine.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/settings.hh>

#include <master.refl.cc>

namespace ce::sdlwindow
{


bool SDLWindowModule::Register(int argc, char **argv, ce::Engine *engine)
{
  register_classes();

  SDLWindow *window = new SDLWindow();
  SDLInputSystem * inputSystem = new SDLInputSystem(window->GetKeyboard(), window->GetMouse());
  ObjectRegistry::Register<iInputSystem>(inputSystem);
  ObjectRegistry::Register<iWindow>(window);
  engine->SetWindow(window);
  return true;
}

bool SDLWindowModule::Initialize(int argc, char **argv, ce::Engine *engine)
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

CE_DEFINE_LIBRARY(ceSDLWindow)
{
  printf ("Load library: ceSDLWindow\n");
  return new ce::sdlwindow::SDLWindowModule;
}