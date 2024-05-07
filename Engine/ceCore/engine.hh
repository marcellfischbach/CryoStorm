//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <vector>

namespace ce
{

struct iDevice;
struct iModule;
struct iFrameRenderer;
struct iRenderTarget2D;
struct iWindow;

class World;

class CE_CORE_API  Engine
{
public:
  void SetWindow(iWindow *window);
  iWindow *GetWindow();

  void SetDevice(iDevice *device);
  iDevice *GetDevice();

  void SetFrameRenderer(iFrameRenderer *frameRenderer);
  iFrameRenderer *GetFrameRenderer();

  void SetWorld(World *world);
  World *GetWorld();

  bool Initialize(int argc, char **argv, ce::iModule *application);

  int Run();
  void Exit(int returnValue = 0);

private:
  iWindow         *m_window        = nullptr;
  iDevice         *m_device        = nullptr;
  iFrameRenderer  *m_frameRenderer = nullptr;
  iRenderTarget2D *m_renderTarget  = nullptr;
  World           *m_world         = nullptr;

  std::vector<iModule *> m_modules;

  bool m_active    = true;
  int  m_exitValue = 0;
public:
  static Engine *Get();

private:
  Engine();

  static Engine *s_instance;
};

} // ce
