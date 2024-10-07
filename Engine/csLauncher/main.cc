
#include <csCore/csEngine.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csVFSConfigReader.hh>
#include <csCore/iGame.hh>
#include <csLauncher/csLauncherModule.hh>
#ifdef CS_WIN32
#include <Windows.h>
#endif

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
  cs::csVFS::Get()->SetRootPath(dataPath);
  cs::csVFSConfigReader::Read(dataPath, configFilename);

  cs::csEngine *engine = cs::csEngine::Get();
  if (!engine->InitializeEngine(args, new cs::launcher::csLauncherModule()))
  {
    printf ("Unable to initialize engine\n");
    return -1;
  };
  if (!engine->InitializeGame())
  {
    printf ("Unable to initialize engine\n");
    return -1;
  };



  return engine->Run();
}



