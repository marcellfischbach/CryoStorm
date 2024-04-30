//
// Created by mcel on 30.04.2024.
//

#include <ceSDLWindow/sdlwindowmodule.hh>
#include <ceSDLWindow/sdlinputsystem.hh>
#include <ceSDLWindow/sdlwindow.hh>

#include <ceCore/engine.hh>
#include <ceCore/objectregistry.hh>

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
  window->Show();
  return true;
}


}
