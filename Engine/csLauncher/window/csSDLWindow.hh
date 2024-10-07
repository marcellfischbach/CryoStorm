//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <csLauncher/window/csSDLMouse.hh>
#include <csLauncher/window/csSDLKeyboard.hh>
#include <csCore/window/iWindow.hh>
#include <SDL.h>

namespace cs::launcher
{




CS_CLASS()
class csSDLWindow : public CS_SUPER(iWindow)
{
CS_CLASS_GEN_OBJECT;

public:
  bool Initialize ();

  void SetTitle(const std::string &title) override;
  const std::string &GetTitle() const override;
  
  void SetPosition(int16_t x, int16_t y) override;
  int GetPositionX() const override;
  int GetPositionY() const override;

  void SetResolution(uint16_t width, uint16_t height) override;
  int GetWidth() const override;
  int GetHeight() const override;

  void SetWindowIcon(const csResourceLocator &locator) override;

  void Show() override;
  void Hide() override;
  void Present() override;
  void ProcessUpdates() override;

  csSDLKeyboard* GetKeyboard();
  csSDLMouse* GetMouse();

private:
  SDL_Window *m_window = nullptr;
  SDL_GLContext m_glContext;

  int16_t    m_posX    = 0;
  int16_t    m_posY    = 0;

  uint16_t m_width  = 1024;
  uint16_t m_height = 768;

  bool m_visible = false;

  std::string m_title = std::string("CryoStorm");

  csSDLKeyboard m_keyboard;
  csSDLMouse    m_mouse;
};

}