//
// Created by mcel on 30.04.2024.
//

#include <ceCore/engine.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/iframerenderer.hh>
#include <ceCore/graphics/defaultframerenderer.hh>
#include <ceCore/window/iwindow.hh>

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


Engine *Engine::s_instance = nullptr;

Engine::Engine()
    : m_frameRenderer(new DefaultFrameRenderer())
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