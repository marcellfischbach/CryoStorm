//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csFPS.hh>
#include <vector>
#include "iGame.hh"

namespace cs
{

struct iDevice;
struct iModule;
struct iFrameRenderer;
struct iRenderTarget2D;
struct iSkyboxRenderer;
struct iWindow;

class csWorld;

class CS_CORE_API  csEngine
{
public:
  void SetWindow(iWindow *window);
  iWindow *GetWindow();

//  void SetDevice(iDevice *device);
//  iDevice *GetDevice();
//
//  void SetFrameRenderer(iFrameRenderer *frameRenderer);
//  iFrameRenderer *GetFrameRenderer();
//
//  void SetSkyboxRenderer(iSkyboxRenderer *skyboxRenderer);
//  iSkyboxRenderer *GetSkyboxRenderer();

  void SetWorld(csWorld *world);
  csWorld *GetWorld();


  bool InitializeEngine(const std::vector<std::string> &args, iModule *externalModule);
  bool InitializeEngine (const std::vector<std::string> &args, const std::vector<std::string> &moduleNames, iModule *externalModule);

  bool InitializeGame();

  int Run();
  bool ProcessFrame(iDevice* device);
  void Exit(int returnValue = 0);

private:
  iWindow         *m_window         = nullptr;
//  iDevice         *m_device         = nullptr;
  iFrameRenderer  *m_frameRenderer  = nullptr;
  iSkyboxRenderer *m_skyboxRenderer = nullptr;
  iRenderTarget2D *m_renderTarget = nullptr;
  csWorld         *m_world        = nullptr;

  bool m_active       = true;
  int  m_exitValue    = 0;
  int  m_multiSamples = 1;


  // Frame counting stuff
  csFPS    m_fps;
  uint32_t m_lastFPS;
public:
  static csEngine *Get();

private:
  csEngine();

  static csEngine *s_instance;
};

} // ce
