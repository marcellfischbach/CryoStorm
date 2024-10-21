//
// Created by mcel on 30.04.2024.
//

#include <csCore/csEngine.hh>
#include <csCore/csFPS.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/csSettings.hh>
#include <csCore/csCoreModule.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iFrameRenderer.hh>
#include <csCore/graphics/csDefaultFrameRenderer.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/iSkyboxRenderer.hh>
#include <csCore/window/iWindow.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csCryoFile.hh>
#include <csCore/resource/csResourceLocator.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/iModule.hh>
#include <csCore/iGame.hh>

#ifdef CS_WIN32

#include <Windows.h>

#endif

namespace cs
{

void csEngine::Exit(int returnValue)
{
  m_exitValue = returnValue;
  shouldExit = true;
}

bool csEngine::ShouldExit() const
{
  return shouldExit;
}

int csEngine::ExitValue() const
{
  return m_exitValue;
}

bool csEngine::InitializeEngine(const std::vector<std::string> &args,
                                const csModuleConfig &config)
{

  if (!csAssetManager::Get())
  {
    csAssetManager::Set(new csAssetManager());
  }

  const std::vector<iModule *> &modules = config.GetModules();

  for (auto module: modules)
  {
    if (!module->Register(args, this))
    {
      return false;
    }
  }

  for (auto module: modules)
  {
    if (!module->Initialize(args, this))
    {
      return false;
    }
  }

  cs::csObjectRegistry::Register<cs::csDebugCache>(new cs::csDebugCache());
  return true;
}






csEngine *csEngine::s_instance = nullptr;


csEngine *csEngine::Get()
{
  if (!s_instance)
  {
    s_instance = new csEngine();
  }
  return s_instance;
}


csModuleConfig::csModuleConfig()
{
  m_modules.push_back(new csCoreModule());
}

bool csModuleConfig::LoadModuleConfig()
{
  return LoadModuleConfigEx("/modules.config");
}

bool csModuleConfig::LoadModuleConfigEx(const std::string &configFilename)
{
  iFile *modulesConfig = csVFS::Get()->Open(csResourceLocator(configFilename), eAM_Read, eOM_Binary);
  if (!modulesConfig)
  {
    return false;
  }

  csCryoFile file;
  if (!file.Parse(modulesConfig))
  {
    CS_RELEASE(modulesConfig);
    return false;
  }

  for (size_t i = 0, in = file.Root()->GetNumberOfChildren(); i < in; i++)
  {
    csCryoFileElement *moduleElement = file.Root()->GetChild(i);
    if (moduleElement)
    {
      AddModuleByName(moduleElement->GetTagName());
    }
  }
  CS_RELEASE(modulesConfig);
  return true;
}



typedef iModule *(*load_library_func_ptr)();


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

static iModule *open_module(const std::string &moduleName)
{
#ifdef  CS_WIN32
  std::string dll_name = moduleName + std::string(".dll");
  std::string load_library_name = moduleName + "_load_library";

  HMODULE handle = load_module(dll_name);
  if (!handle)
  {
    return nullptr;
  }


  load_library_func_ptr load_library_func = (load_library_func_ptr) GetProcAddress(handle, load_library_name.c_str());
  if (!load_library_func)
  {
    return nullptr;
  }

  iModule *module = load_library_func();

  return module;
#else
  return nullptr;
#endif
}

void csModuleConfig::AddModuleByName(const std::string &moduleName)
{
  iModule *module = open_module(moduleName);
  AddModule(module);
}

void csModuleConfig::AddModule(cs::iModule *module)
{
  if (module)
  {
    if (std::find(m_modules.begin(), m_modules.end(), module) == m_modules.end())
    {
      module->AddRef();
      m_modules.push_back(module);
    }
  }
}

const std::vector<iModule *> &csModuleConfig::GetModules() const
{
  return m_modules;
}

} // ce