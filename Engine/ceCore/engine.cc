//
// Created by mcel on 30.04.2024.
//

#include <ceCore/engine.hh>
#include <ceCore/fps.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/settings.hh>
#include <ceCore/coremodule.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/iframerenderer.hh>
#include <ceCore/graphics/defaultframerenderer.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/iskyboxrenderer.hh>
#include <ceCore/window/iwindow.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/file.hh>
#include <ceCore/resource/resourcelocator.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/imodule.hh>
#include <ceCore/igame.hh>

#ifdef CS_WIN32

#include <Windows.h>

#endif

namespace ce
{


void Engine::SetDevice(ce::iDevice *device)
{
  CS_SET(m_device, device);
}

iDevice *Engine::GetDevice()
{
  return m_device;
}

void Engine::SetFrameRenderer(ce::iFrameRenderer *frameRenderer)
{
  CS_SET(m_frameRenderer, frameRenderer);
}

iFrameRenderer *Engine::GetFrameRenderer()
{
  return m_frameRenderer;
}

void Engine::SetSkyboxRenderer(ce::iSkyboxRenderer *skyboxRenderer)
{
  CS_SET(m_skyboxRenderer, skyboxRenderer);
}

iSkyboxRenderer *Engine::GetSkyboxRenderer()
{
  return m_skyboxRenderer;
}

void Engine::SetWindow(ce::iWindow *window)
{
  CS_SET(m_window, window);
}

iWindow *Engine::GetWindow()
{
  return m_window;
}

void Engine::SetWorld(ce::World *world)
{
  CS_SET(m_world, world);
}

World *Engine::GetWorld()
{
  return m_world;
}

void Engine::Exit(int returnValue)
{
  m_exitValue = returnValue;
  m_active    = false;
}

typedef iModule *(*load_library_func_ptr)();


struct OpenModule
{
  iModule *module;
  iGame   *game;
};

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

  printf("ceEngine::Module [%s]: %p\n", libraryName.c_str(), handle);

  return handle;
}

iModule *open_module(const std::string &moduleName)
{
#ifdef  CS_WIN32
  std::string dll_name          = moduleName + std::string(".dll");
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


iGame *open_game(const std::string &moduleName)
{
#ifdef  CS_WIN32
  std::string dll_name             = moduleName + std::string(".dll");
  std::string create_game_instance = "create_game_instance";

  HMODULE handle = load_module(dll_name);
  if (!handle)
  {
    return nullptr;
  }

  create_game_instance_func_ptr create_game_instance_func = (create_game_instance_func_ptr) GetProcAddress(handle,
                                                                                                           create_game_instance
                                                                                                               .c_str());
  if (!create_game_instance_func)
  {
    return nullptr;
  }

  return create_game_instance_func();
#else
  return nullptr;
#endif
}


ce::iRenderTarget2D *create_render_target(ce::iDevice *device, uint32_t width, uint32_t height, uint16_t multiSamples)
{
  ce::iSampler *colorSampler = device->CreateSampler();
  colorSampler->SetFilterMode(ce::eFM_MinMagNearest);

  ce::iSampler *depthSampler = device->CreateSampler();
  depthSampler->SetFilterMode(ce::eFM_MinMagNearest);
  depthSampler->SetTextureCompareFunc(ce::eCF_LessOrEqual);
  depthSampler->SetTextureCompareMode(ce::eTCM_None);

  ce::iTexture2D::Descriptor rt_col_desc = {};
  rt_col_desc.Width        = width;
  rt_col_desc.Height       = height;
  rt_col_desc.Format       = ce::ePF_RGBA;
  rt_col_desc.MipMaps      = false;
  rt_col_desc.MultiSamples = multiSamples;
  ce::iTexture2D *color_texture = device->CreateTexture(rt_col_desc);
  color_texture->SetSampler(colorSampler);

  ce::iTexture2D::Descriptor rt_dpth_desc = {};
  rt_dpth_desc.Width        = width;
  rt_dpth_desc.Height       = height;
  rt_dpth_desc.Format       = ce::ePF_DepthStencil;
  rt_dpth_desc.MipMaps      = false;
  rt_dpth_desc.MultiSamples = multiSamples;
  ce::iTexture2D *depth_texture = device->CreateTexture(rt_dpth_desc);
  depth_texture->SetSampler(depthSampler);


  ce::iRenderTarget2D::Descriptor rt_desc = {};
  rt_desc.Width  = width;
  rt_desc.Height = height;

  ce::iRenderTarget2D *renderTarget = device->CreateRenderTarget(rt_desc);
  renderTarget->AddColorTexture(color_texture);
//  renderTarget->SetDepthBuffer(ce::ePF_Depth);
  renderTarget->SetDepthTexture(depth_texture);
  if (!renderTarget->Compile())
  {
    printf("Unable to compile render target: %s\n", renderTarget->GetCompileLog().c_str());
    return nullptr;
  }
  return renderTarget;
}


bool Engine::InitializeEngine(const std::vector<std::string> &args, iModule *externalModule)
{
  iFile *modulesConfig = VFS::Get()->Open(ResourceLocator("/modules.config"), eAM_Read, eOM_Binary);
  if (!modulesConfig)
  {
    return false;
  }

  CrimsonFile file;
  if (!file.Parse(modulesConfig))
  {
    CS_RELEASE(modulesConfig);
    return false;
  }

  std::vector<std::string> moduleNames;
  for (size_t              i = 0, in = file.Root()->GetNumberOfChildren(); i < in; i++)
  {
    CrimsonFileElement *moduleElement = file.Root()->GetChild(i);
    if (moduleElement)
    {
      moduleNames.emplace_back(moduleElement->GetTagName());
    }
  }
  CS_RELEASE(modulesConfig);


  return InitializeEngine(args, moduleNames, externalModule);
}

bool Engine::InitializeEngine(const std::vector<std::string> &args,
                              const std::vector<std::string> &moduleNames,
                              ce::iModule *externalModule)
{

  if (!AssetManager::Get())
  {
    AssetManager::Set(new AssetManager());
  }

  std::vector<iModule *> modules;
  modules.push_back(new CoreModule());
  if (externalModule)
  {
    modules.push_back(externalModule);
  }

  for (auto &moduleName: moduleNames)
  {
    iModule *module = open_module(moduleName);
    if (module)
    {
      modules.emplace_back(module);
    }
  }

  if (std::find(moduleNames.begin(), moduleNames.end(), "ceGame") == moduleNames.end())
  {
    iModule *gameModule = open_module("ceGame");
    if (gameModule)
    {
      modules.emplace_back(gameModule);
    }
  }

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

  ce::ObjectRegistry::Register<ce::DebugCache>(new ce::DebugCache());
  m_multiSamples = Settings::Get().Graphics().GetInt("multisamples", 1);

  return true;
}

bool Engine::InitializeGame()
{

  if (!m_world)
  {
    m_world = new World();
  }

  iGame* game = open_game("ceGame");
  if (game)
  {
    game->Initialize(this);
  }
  return true;
}

int Engine::Run()
{

#if _DEBUG
  size_t numDrawCallsPerSec    = 0;
  size_t numTrianglesPerSec    = 0;
  size_t numShaderStateChanges = 0;
#endif

  while (m_active)
  {
#if _DEBUG
    m_device->ResetDebug();
#endif

    m_window->ProcessUpdates();
    if (!ProcessFrame())
    {
      m_active = false;
      m_exitValue = -1;
      break;
    }
    m_window->Present();

#if _DEBUG
    numDrawCallsPerSec += m_device->GetNumberOfDrawCalls();
    numTrianglesPerSec += m_device->GetNumberOfTriangles();
    numShaderStateChanges += m_device->GetNumberOfShaderStateChanges();
#endif

  }
  return m_exitValue;
}

bool Engine::ProcessFrame()
{
  if (!m_renderTarget || m_renderTarget->GetWidth() != m_window->GetWidth() || m_renderTarget->GetHeight() != m_window->GetHeight())
  {
    CS_RELEASE(m_renderTarget);
    m_renderTarget = create_render_target(m_device, m_window->GetWidth(), m_window->GetHeight(), m_multiSamples);
    if (m_renderTarget == nullptr)
    {
      return false;
    }
  }

  int64_t frameTime = m_fps.Tick();
  if (frameTime != 0)
  {
    uint32_t currentFPS = m_fps.Get();
    if (currentFPS != m_lastFPS)
    {
      m_lastFPS = currentFPS;

      std::string title = std::string("CrimsonEdge ") + std::to_string(currentFPS) + std::string(" FPS");
      printf("%s\n", title.c_str());
      fflush(stdout);
      m_window->SetTitle(title);
    }

    float tpf = (float) frameTime / 1000.0f;

    m_world->Update(tpf);
  }


  m_frameRenderer->Render(m_renderTarget, m_device, m_world->GetScene());

  ce::iTexture2D *finalColor = m_renderTarget->GetColorTexture(0);

  m_device->SetRenderTarget(nullptr);
  m_device->SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
  m_device->SetDepthTest(false);
  m_device->SetBlending(false);
  m_device->RenderFullscreen(finalColor);
  m_device->SetDepthTest(true);

  return true;
}


Engine *Engine::s_instance = nullptr;

Engine::Engine()
    : m_frameRenderer(new DefaultFrameRenderer())
    , m_world(nullptr)
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