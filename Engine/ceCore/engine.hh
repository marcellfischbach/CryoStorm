//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/fps.hh>
#include <vector>
#include "igame.hh"

namespace ce
{

struct iDevice;
struct iModule;
struct iFrameRenderer;
struct iRenderTarget2D;
struct iSkyboxRenderer;
struct iWindow;

class World;

class CS_CORE_API  Engine
{
public:
  void SetWindow(iWindow *window);
  iWindow *GetWindow();

  void SetDevice(iDevice *device);
  iDevice *GetDevice();

  void SetFrameRenderer(iFrameRenderer *frameRenderer);
  iFrameRenderer *GetFrameRenderer();

  void SetSkyboxRenderer(iSkyboxRenderer *skyboxRenderer);
  iSkyboxRenderer *GetSkyboxRenderer();

  void SetWorld(World *world);
  World *GetWorld();


  bool InitializeEngine(const std::vector<std::string> &args, iModule *externalModule);
  bool InitializeEngine (const std::vector<std::string> &args, const std::vector<std::string> &moduleNames, iModule *externalModule);

  bool InitializeGame();

  int Run();
  bool ProcessFrame();
  void Exit(int returnValue = 0);

private:
  iWindow         *m_window         = nullptr;
  iDevice         *m_device         = nullptr;
  iFrameRenderer  *m_frameRenderer  = nullptr;
  iSkyboxRenderer *m_skyboxRenderer = nullptr;
  iRenderTarget2D *m_renderTarget   = nullptr;
  World           *m_world          = nullptr;

  bool m_active       = true;
  int  m_exitValue    = 0;
  int  m_multiSamples = 1;


  // Frame counting stuff
  FPS      m_fps;
  uint32_t m_lastFPS;
public:
  static Engine *Get();

private:
  Engine();

  static Engine *s_instance;
};

} // ce
