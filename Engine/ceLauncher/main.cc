
#include <ceCore/engine.hh>
#include <ceLauncher/launchermodule.hh>

int main(int argc, char** argv)
{
  ce::Engine *engine = ce::Engine::Get();

  if (!engine->Initialize(argc, argv, new ce::launcher::LauncherModule()))
  {
    printf ("Unable to initialize engine\n");
    return -1;
  };


  return engine->Run();
}



