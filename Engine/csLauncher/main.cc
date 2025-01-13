
#include <csCore/csEngine.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csVFSConfigReader.hh>
#include <csCore/iGame.hh>
#include <csLauncher/csLauncherModule.hh>

#ifdef CS_WIN32

#include <Windows.h>

#endif

#include <csCore/iGame.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/csViewport.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iFrameRenderer.hh>
#include <csCore/window/iWindow.hh>


using namespace cs;
using namespace cs::launcher;

static HMODULE load_library(const std::string &libraryName)
{
  HMODULE handle = GetModuleHandle(libraryName.c_str());
  if (!handle)
  {
    handle = LoadLibraryEx(libraryName.c_str(), nullptr, 0);
  }

  return handle;
}


static HMODULE load_module(const std::string &libraryName)
{
  HMODULE handle = load_library(libraryName);
  if (!handle)
  {
    handle = load_library(std::string("lib") + libraryName);
  }

  return handle;
}

cs::iGame *open_game(const std::string &moduleName)
{
#ifdef  CS_WIN32
  std::string dll_name = moduleName + std::string(".dll");
  std::string create_game_instance = "create_game_instance";

  HMODULE handle = load_module(dll_name);
  if (!handle)
  {
    return nullptr;
  }

  cs::create_game_instance_func_ptr create_game_instance_func = (cs::create_game_instance_func_ptr) GetProcAddress(
      handle,
      create_game_instance.c_str()
  );
  if (!create_game_instance_func)
  {
    return nullptr;
  }

  return create_game_instance_func();
#else
  return nullptr;
#endif
}


struct base_t
{
  base_t()
  {
    printf("base_t::base_t\n");
  }
};

struct derive_a_t : public virtual base_t
{

  derive_a_t()
  {
    printf("derive_a_t::derive_a_t\n");
  }

};


struct derive_b_t : public virtual derive_a_t
{


  derive_b_t()
  {
    printf("derive_b_t::derive_b_t\n");
  }
};


struct derive_c_t : public virtual derive_b_t
{

  derive_c_t()
  {
    printf("derive_c_t::derive_c_t\n");
  }
};


#include <csCore/resource/iFile.hh>
#include <csCore/resource/csFileSystemFile.hh>
#include <csCore/csRef.hh>




int main(int argc, char **argv)
{

  std::vector<std::string> args;
  for (int i = 0; i < argc; i++)
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


  printf("Starting with data-path: '%s'\n", dataPath.c_str());
  fflush(stdout);
  cs::csVFS::Get()->SetRootPath(dataPath);
  cs::csVFSConfigReader::Read(dataPath, configFilename);

  cs::csEngine *engine = cs::csEngine::Get();

  cs::csModuleConfig config;
  config.AddModule(new cs::launcher::csLauncherModule());
  config.LoadModuleConfig();

  if (!engine->InitializeEngine(args, config))
  {
    printf("Unable to initialize engine\n");
    return -1;
  };


  iWindow *window = csObjectRegistry::Get<iWindow>();

  csViewport *viewport = new csViewport();
  viewport->SetWindow(window);
  viewport->SetDevice(csObjectRegistry::Get<iDevice>());
  viewport->SetWorld(new csWorld());
  viewport->SetFrameRenderer(csObjectRegistry::Get<iFrameRenderer>());


  cs::iGame *game = open_game("csGame");
  if (game)
  {
    game->Initialize(viewport->GetWorld());
  }


  bool active = true;
  int exitValue = 0;
  while (active)
  {
    window->ProcessUpdates();
    if (!viewport->ProcessFrame(nullptr))
    {
      active = false;
      exitValue = -1;
    }
    else if (engine->ShouldExit())
    {
      active = false;
      exitValue = engine->ExitValue();
    }
    window->Present();
  }

  return exitValue;
}



