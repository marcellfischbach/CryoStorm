//
// Created by Marcell on 20.10.2024.
//

#include <csCore/csViewport.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iFrameRenderer.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/iSkyboxRenderer.hh>
#include <csCore/window/iWindow.hh>


namespace cs
{

csViewport::~csViewport()
{
  CS_RELEASE(m_device);
  CS_RELEASE(m_world);
  CS_RELEASE(m_frameRenderer);
  CS_RELEASE(m_skyboxRenderer);
  CS_RELEASE(m_renderTarget);
  CS_RELEASE(m_window);
}

void csViewport::SetDevice(cs::iDevice *device)
{
  CS_SET(m_device, device);
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
  CS_SET(m_world, world);
}

cs::csWorld *csViewport::GetWorld()
{
  return m_world;
}

const cs::csWorld *csViewport::GetWorld() const
{
  return m_world;
}


void csViewport::SetFrameRenderer(cs::iFrameRenderer *frameRenderer)
{
  CS_SET(m_frameRenderer, frameRenderer);
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
  CS_SET(m_skyboxRenderer, skyboxRenderer);
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
  CS_SET(m_window, window);
}

cs::iWindow *csViewport::GetWindow()
{
  return m_window;
}

const cs::iWindow *csViewport::GetWindow() const
{
  return m_window;
}

} // cs