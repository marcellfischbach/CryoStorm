//
// Created by mcel on 30.04.2024.
//

#include <ceCore/engine.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/iframerenderer.hh>
#include <ceCore/graphics/defaultframerenderer.hh>
#include <ceCore/window/iwindow.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/file.hh>
#include <ceCore/resource/resourcelocator.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/imodule.hh>

#ifdef CE_WIN32

#include <Windows.h>

#endif

namespace ce
{


void Engine::SetDevice(ce::iDevice *device)
{
  CE_SET(m_device, device);
}

iDevice *Engine::GetDevice()
{
  return m_device;
}

void Engine::SetFrameRenderer(ce::iFrameRenderer *frameRenderer)
{
  CE_SET(m_frameRenderer, frameRenderer);
}

iFrameRenderer *Engine::GetFrameRenderer()
{
  return m_frameRenderer;
}

void Engine::SetWindow(ce::iWindow *window)
{
  CE_SET(m_window, window);
}

iWindow *Engine::GetWindow()
{
  return m_window;
}

void Engine::SetWorld(ce::World *world)
{
  CE_SET(m_world, world);
}

World* Engine::GetWorld()
{
  return m_world;
}

typedef iModule *(*load_library_func_ptr)();

iModule *open_module(CrimsonFileElement *moduleElement)
{
#ifdef  CE_WIN32
  std::string lib_name              = moduleElement->GetTagName();
  std::string dll_name          = lib_name + std::string(".dll");
  std::string load_library_name = lib_name + "_load_library";

  HMODULE handle = GetModuleHandle(dll_name.c_str());
  if (!handle)
  {
    handle = LoadLibraryEx(dll_name.c_str(), nullptr, 0);
    if (!handle)
    {
      return nullptr;
    }
  }


  load_library_func_ptr load_library_func = (load_library_func_ptr) GetProcAddress(handle, load_library_name.c_str());
  if (!load_library_func)
  {
    return nullptr;
  }

  return load_library_func();
#else
  return nullptr;
#endif
}

bool Engine::Initialize(int argc, char **argv, ce::iModule *application)
{
  std::string basePath("../");
  for (int    i = 0; i < argc; i++)
  {
    std::string arg(argv[i]);
    if (arg == std::string("--data") && i + 1 < argc)
    {
      basePath = std::string(argv[++i]);
    }
  }
  printf("Starting with base-path: '%s'\n", basePath.c_str());
  ce::VFS::Get()->SetRootPath(basePath);

  if (!AssetManager::Get())
  {
    AssetManager::Set(new AssetManager());
  }

  iFile *modulesConfig = VFS::Get()->Open(ResourceLocator("/modules.config"), eAM_Read, eOM_Text);
  if (!modulesConfig)
  {
    return false;
  }

  CrimsonFile file;
  if (!file.Parse(modulesConfig))
  {
    CE_RELEASE(modulesConfig);
    return false;
  }

  for (size_t i = 0, in = file.Root()->GetNumberOfChildren(); i < in; i++)
  {
    CrimsonFileElement *moduleElement = file.Root()->GetChild(i);
    if (moduleElement)
    {
      iModule *module = open_module(moduleElement);
      if (module)
      {
        m_modules.push_back(module);
      }
    }
  }

  if (application)
  {
    m_modules.push_back(application);
  }

  for (auto module: m_modules)
  {
    if (!module->Register(argc, argv, this))
    {
      return false;
    }
  }

  for (auto module: m_modules)
  {
    if (!module->Initialize(argc, argv, this))
    {
      return false;
    }
  }


  CE_RELEASE(modulesConfig);

  if (!m_world)
  {
    m_world = new World();
  }

  return true;
}

int Engine::Run()
{
  while (true)
  {
#if _DEBUG
    m_device->ResetDebug();
#endif


  }
}


Engine *Engine::s_instance = nullptr;

Engine::Engine()
    : m_frameRenderer(new DefaultFrameRenderer())
    , m_world (nullptr)
{

}

Engine *Engine::Get()
{
  if (!s_instance)
  {
    s_instance = new Engine();
  }
  return s_instance;
}

} // ce