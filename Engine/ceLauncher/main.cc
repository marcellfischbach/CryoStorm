
#include <ceCore/engine.hh>
#include <ceLauncher/launchermodule.hh>

int main(int argc, char** argv)
{
  std::vector<std::string> args;
  for (int i=0; i<argc; i++)
  {
    args.emplace_back(argv[i]);
  }

  ce::Engine *engine = ce::Engine::Get();

  if (!engine->Initialize(args, new ce::launcher::LauncherModule()))
  {
    printf ("Unable to initialize engine\n");
    return -1;
  };


  return engine->Run();
}



