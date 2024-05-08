//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <ceLauncher/window/sdlmouse.hh>
#include <ceLauncher/window/sdlkeyboard.hh>
#include <ceCore/window/iwindow.hh>
#include <SDL.h>

namespace ce::launcher
{




CE_CLASS()
class SDLWindow : public CE_SUPER(iWindow)
{
CE_CLASS_GEN_OBJECT;

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

  void SetWindowIcon (const ResourceLocator &locator) override;

  void Show() override;
  void Hide() override;
  void Present() override;
  void ProcessUpdates() override;

  SDLKeyboard* GetKeyboard();
  SDLMouse* GetMouse();

private:
  SDL_Window *m_window = nullptr;
  SDL_GLContext m_glContext;

  int16_t    m_posX    = 0;
  int16_t    m_posY    = 0;

  uint16_t m_width  = 1024;
  uint16_t m_height = 768;

  bool m_visible = false;

  std::string m_title = std::string("CrimsonEdge");

  SDLKeyboard m_keyboard;
  SDLMouse    m_mouse;
};

}