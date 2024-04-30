//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <ceCore/coreexport.hh>


namespace ce
{

struct iDevice;
struct iFrameRenderer;
struct iWindow;

class CE_CORE_API  Engine
{
public:
  void SetWindow(iWindow *window);
  iWindow *GetWindow();

  void SetDevice(iDevice *device);
  iDevice *GetDevice();

  void SetFrameRenderer(iFrameRenderer *frameRenderer);
  iFrameRenderer * GetFrameRenderer();


private:
  iWindow *m_window = nullptr;

  iDevice *m_device = nullptr;

  iFrameRenderer *m_frameRenderer = nullptr;

public:
  static Engine *Get();
private:
  Engine();

  static Engine *s_instance;
};

} // ce
