//
// Created by mcel on 30.04.2024.
//

#include <ceCore/engine.hh>
#include <ceCore/fps.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/settings.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/iframerenderer.hh>
#include <ceCore/graphics/defaultframerenderer.hh>
#include <ceCore/graphics/irendertarget2d.hh>
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

World *Engine::GetWorld()
{
  return m_world;
}

void Engine::Exit(int returnValue)
{
  m_exitValue = returnValue;
  m_active = false;
}

typedef iModule *(*load_library_func_ptr)();

iModule *open_module(CrimsonFileElement *moduleElement)
{
#ifdef  CE_WIN32
  std::string lib_name          = moduleElement->GetTagName();
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
  printf("CreateDepthTexture: %p\n", depth_texture);


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


  int multiSamples = Settings::Get().Display().GetInt("multisamples", 1);
  m_renderTarget = create_render_target(m_device, m_window->GetWidth(), m_window->GetHeight(), multiSamples);

  ce::ObjectRegistry::Register<ce::DebugCache>(new ce::DebugCache());


  return m_renderTarget != nullptr;
}

int Engine::Run()
{
  FPS      fps;
  uint32_t lastFPS = 0;

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


    int64_t frameTime = fps.Tick();
    if (frameTime != 0)
    {
      uint32_t currentFPS = fps.Get();
      if (currentFPS != lastFPS)
      {
        lastFPS = currentFPS;

        std::string title = std::string("CrimsonEdge ") + std::to_string(currentFPS) + std::string(" FPS");
        m_window->SetTitle(title);
        printf("%s\n", title.c_str());
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


#if _DEBUG
    numDrawCallsPerSec += m_device->GetNumberOfDrawCalls();
    numTrianglesPerSec += m_device->GetNumberOfTriangles();
    numShaderStateChanges += m_device->GetNumberOfShaderStateChanges();
#endif

    m_window->Present();
  }
  return m_exitValue;
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