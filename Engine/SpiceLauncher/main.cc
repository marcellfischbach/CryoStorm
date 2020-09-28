
#include <iostream>
#include <SpiceCore/objectregistry.hh>
#include <SpiceCore/window/iwindow.hh>
#include <SpiceSDLWindow/sdlwindowmodule.hh>



int main(int argc, char** argv)
{
  Spice::SDLWindowModule::Initialize (argc, argv);

  Spice::iWindow *wnd = Spice::ObjectRegistry::Get()->Get<Spice::iWindow>();
  if (wnd == nullptr)
  {
    printf ("Window not registered\n");
    return -1;
  }
  wnd->SetTitle("Spice Window");
  wnd->SetPosition(50, 50);
  wnd->SetResolution(1024, 768);
  wnd->Show();

  while (true)
  {
    wnd->ProcessUpdates();
    wnd->Present();
  }


  return 0;
}
