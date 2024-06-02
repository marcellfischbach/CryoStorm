
#include <ceCore/engine.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/resource/vfsconfigreader.hh>
#include <ceLauncher/launchermodule.hh>

int main(int argc, char** argv)
{
  std::vector<std::string> args;
  for (int i=0; i<argc; i++)
  {
    args.emplace_back(argv[i]);
  }


  std::string dataPath("../");
  std::string configFilename = "vfs.config";
  for (size_t i = 0, in = args.size(); i < in; i++)
  {
    const std::string &arg(args[i]);
    if (arg == std::string("--data") && i + 1 < in)
    {
      dataPath = std::string(args[++i]);
    }
    else if (arg == std::string("--config") && i + 1 < in)
    {
      configFilename = std::string(args[++i]);
    }
  }


  printf("Starting with data-path: '%s'\n", dataPath.c_str()); fflush(stdout);
  ce::VFS::Get()->SetRootPath(dataPath);
  ce::VFSConfigReader::Read(dataPath, configFilename);

  ce::Engine *engine = ce::Engine::Get();
  if (!engine->Initialize(args, new ce::launcher::LauncherModule(), nullptr))
  {
    printf ("Unable to initialize engine\n");
    return -1;
  };


  return engine->Run();
}



