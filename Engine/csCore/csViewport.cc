//
// Created by Marcell on 20.10.2024.
//

#include <csCore/csViewport.hh>
#include <csCore/csSettings.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iFrameRenderer.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/iSkyboxRenderer.hh>
#include <csCore/window/iWindow.hh>
#include <csCore/csObjectRegistry.hh>


namespace cs
{

csViewport::csViewport()
{
  m_multiSamples = csSettings::Get().Graphics().GetInt("multisamples", 1);
}

csViewport::~csViewport()
{
}

void csViewport::SetRenderPipeline(eRenderPipeline pipeline)
{
  m_pipeline = pipeline;
  InitializeFrameRenderer();
}

eRenderPipeline csViewport::GetRenderPipeline() const
{
  return m_pipeline;
}

void csViewport::SetDevice(cs::iDevice *device)
{
  m_device = device;
}

iDevice *csViewport::GetDevice()
{
  return m_device;
}

const iDevice *csViewport::GetDevice() const
{
  return m_device;
}

void csViewport::SetWorld(cs::csWorld *world)
{
  m_world = world;
}

cs::csWorld *csViewport::GetWorld()
{
  return m_world;
}

const cs::csWorld *csViewport::GetWorld() const
{
  return m_world;
}


cs::iFrameRenderer *csViewport::GetFrameRenderer()
{
  return m_frameRenderer;
}

const cs::iFrameRenderer *csViewport::GetFrameRenderer() const
{
  return m_frameRenderer;
}

void csViewport::SetSkyboxRenderer(cs::iSkyboxRenderer *skyboxRenderer)
{
  m_skyboxRenderer = skyboxRenderer;
}

cs::iSkyboxRenderer *csViewport::GetSkyboxRenderer()
{
  return m_skyboxRenderer;
}

const cs::iSkyboxRenderer *csViewport::GetSkyboxRenderer() const
{
  return m_skyboxRenderer;
}

cs::iRenderTarget2D *csViewport::GetRenderTarget()
{
  return m_renderTarget;
}

const cs::iRenderTarget2D *csViewport::GetRenderTarget() const
{
  return m_renderTarget;
}

void csViewport::SetWindow(cs::iWindow *window)
{
  m_window = window;
}

cs::iWindow *csViewport::GetWindow()
{
  return m_window;
}

const cs::iWindow *csViewport::GetWindow() const
{
  return m_window;
}


bool csViewport::InitializeFrameRenderer()
{
  iFrameRendererFactory* factory = csObjectRegistry::Get<iFrameRendererFactory>();
  if (!factory)
  {
    return false;
  }


  m_frameRenderer = factory->Create(m_pipeline);
  return m_frameRenderer != nullptr;
}

static csOwned<cs::iRenderTarget2D>
create_render_target(cs::iDevice *device, uint32_t width, uint32_t height, uint16_t multiSamples)
{
  csRef<cs::iSampler> colorSampler = device->CreateSampler();
  colorSampler->SetFilterMode(cs::eFM_MinMagNearest);

  csRef<cs::iSampler> depthSampler = device->CreateSampler();
  depthSampler->SetFilterMode(cs::eFM_MinMagNearest);
  depthSampler->SetTextureCompareFunc(cs::eCF_LessOrEqual);
  depthSampler->SetTextureCompareMode(cs::eTCM_None);

  cs::iTexture2D::Descriptor rt_col_desc = {};
  rt_col_desc.Width = width;
  rt_col_desc.Height = height;
  rt_col_desc.Format = cs::ePF_RGBA;
  rt_col_desc.MipMaps = false;
  rt_col_desc.MultiSamples = multiSamples;
  csRef<cs::iTexture2D> color_texture = device->CreateTexture(rt_col_desc);
  color_texture->SetSampler(colorSampler);

  cs::iTexture2D::Descriptor rt_dpth_desc = {};
  rt_dpth_desc.Width = width;
  rt_dpth_desc.Height = height;
  rt_dpth_desc.Format = cs::ePF_DepthStencil;
  rt_dpth_desc.MipMaps = false;
  rt_dpth_desc.MultiSamples = multiSamples;
  csRef<cs::iTexture2D> depth_texture = device->CreateTexture(rt_dpth_desc);
  depth_texture->SetSampler(depthSampler);


  cs::iRenderTarget2D::Descriptor rt_desc = {};
  rt_desc.Width = width;
  rt_desc.Height = height;

  csRef<cs::iRenderTarget2D> renderTarget = device->CreateRenderTarget(rt_desc);
  renderTarget->AddColorTexture(color_texture);
//  renderTarget->SetDepthBuffer(cs::ePF_Depth);
  renderTarget->SetDepthTexture(depth_texture);
  if (!renderTarget->Compile())
  {
    printf("Unable to compile render target: %s\n", renderTarget->GetCompileLog().c_str());
    return nullptr;
  }
  return csOwned<iRenderTarget2D>(renderTarget);
}


bool csViewport::ProcessFrame(iRenderTarget2D *renderTarget)
{
  m_device->CheckError();

  if (!m_frameRenderer)
  {
    if (!InitializeFrameRenderer())
    {
      return false;
    }
  }

  if (!m_renderTarget || m_renderTarget->GetWidth() != m_window->GetWidth() ||
      m_renderTarget->GetHeight() != m_window->GetHeight())
  {
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

      std::string title = std::string("CryoStorm ") + std::to_string(currentFPS) + std::string(" FPS");
//      printf("%s\n", title.c_str());
//      fflush(stdout);
      m_window->SetTitle(title);
    }

    float tpf = (float) frameTime / 1000.0f;

    m_world->Update(tpf);
  }
  m_device->CheckError();


  m_frameRenderer->Render(m_renderTarget, m_device, m_world->GetScene());

  cs::iTexture2D *finalColor = m_renderTarget->GetColorTexture(0);

  if (renderTarget)
  {
    m_device->SetRenderTarget(renderTarget);
  }
  else
  {
    m_device->SetRenderTarget(nullptr);
    m_device->SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
  }
  m_device->SetDepthTest(false);
  m_device->SetBlending(false);
  m_device->RenderFullscreen(finalColor);
  m_device->SetDepthTest(true);

  return true;
}


} // cs


#ifdef CS_JAVA

#include <jni.h>
#include <jawt.h>
#include <jawt_md.h>

extern "C"
{
JNIEXPORT void
JNICALL Java_org_cryo_demo_TestView_paint(JNIEnv *env, jclass, jintArray buffer)
{
  int length = env->GetArrayLength(buffer);
  jboolean isCopy = false;
  jint* intS = env->GetIntArrayElements(buffer, &isCopy);

  for (int i=0; i<length; i++)
  {
    intS[i] = 0xff008000;
  }
  env->ReleaseIntArrayElements(buffer, intS, 0);
}


};
#endif

