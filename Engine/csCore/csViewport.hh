//
// Created by Marcell on 20.10.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csFPS.hh>

namespace cs
{

class csWorld;
struct iDevice;
struct iFrameRenderer;
struct iSkyboxRenderer;
struct iRenderTarget2D;
struct iWindow;


CS_CLASS(jclass="org.cryo.core.Viewport")
class CS_CORE_API csViewport : public CS_SUPER(cs::iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  csViewport();
  ~csViewport() override;

  CS_FUNCTION()
  void SetDevice(cs::iDevice *device);
  CS_FUNCTION()
  CS_NODISCARD cs::iDevice *GetDevice();
  CS_NODISCARD const iDevice *GetDevice() const;

  CS_FUNCTION()
  void SetWorld(cs::csWorld *world);
  CS_FUNCTION()
  CS_NODISCARD cs::csWorld *GetWorld();
  CS_NODISCARD const cs::csWorld *GetWorld() const;


  CS_FUNCTION()
  void SetFrameRenderer(cs::iFrameRenderer *frameRenderer);
  CS_FUNCTION()
  CS_NODISCARD cs::iFrameRenderer *GetFrameRenderer();
  CS_NODISCARD const cs::iFrameRenderer *GetFrameRenderer() const;

  CS_FUNCTION()
  void SetSkyboxRenderer(cs::iSkyboxRenderer *skyboxRenderer);
  CS_FUNCTION()
  CS_NODISCARD cs::iSkyboxRenderer *GetSkyboxRenderer();
  CS_NODISCARD const cs::iSkyboxRenderer *GetSkyboxRenderer() const;

  CS_FUNCTION()
  CS_NODISCARD cs::iRenderTarget2D *GetRenderTarget();
  CS_NODISCARD const cs::iRenderTarget2D *GetRenderTarget() const;

  CS_FUNCTION()
  void SetWindow(cs::iWindow *window);
  CS_FUNCTION()
  CS_NODISCARD cs::iWindow *GetWindow();
  CS_NODISCARD const cs::iWindow *GetWindow() const;


  CS_FUNCTION()
  bool ProcessFrame (cs::iRenderTarget2D *target);

private:
  csFPS    m_fps;
  uint32_t m_lastFPS;

  iDevice *m_device = nullptr;
  csWorld *m_world = nullptr;
  iFrameRenderer *m_frameRenderer = nullptr;
  iSkyboxRenderer *m_skyboxRenderer = nullptr;
  iRenderTarget2D *m_renderTarget = nullptr;
  iWindow *m_window = nullptr;

  uint8_t m_multiSamples = 1;
};

} // cs
