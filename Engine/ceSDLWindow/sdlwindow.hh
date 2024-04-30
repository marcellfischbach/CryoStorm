//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <ceSDLWindow/sdlwindowexport.hh>
#include <ceSDLWindow/sdlmouse.hh>
#include <ceSDLWindow/sdlkeyboard.hh>
#include <ceCore/window/iwindow.hh>
#include <SDL.h>

namespace ce::sdlwindow
{




CE_CLASS()
class CE_SDL_API SDLWindow : public CE_SUPER(iWindow)
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