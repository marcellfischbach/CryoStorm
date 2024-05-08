#pragma once


#include <ceCore/input/imouse.hh>
#include <SDL.h>

namespace ce::launcher
{


class SDLMouse : public iMouse
{
public:
  SDLMouse();
  ~SDLMouse() override = default;

  void SetWindow (SDL_Window *window);

  void Update();
  void Update(uint8_t button, bool down);
  void Update(int32_t wheel, int32_t wheelHorizontal);
  void Update(int32_t x, int32_t y, int32_t xrel, int32_t yrel);


  void SetVisible (bool visible) override;
  CE_NODISCARD  bool IsVisible () const override;
  void SetCursorMode (eCursorMode mode) override;
  CE_NODISCARD eCursorMode GetCursorMode () const override;


  CE_NODISCARD int32_t GetX() const override;
  CE_NODISCARD int32_t GetY() const override;

  CE_NODISCARD int32_t GetDeltaX() const override;
  CE_NODISCARD int32_t GetDeltaY() const override;
  CE_NODISCARD int32_t GetWheel() const override;
  CE_NODISCARD int32_t GetWheelHorizontal() const override;


  CE_NODISCARD bool IsButtonDown(eMouseButton mouseButton) const override;
  CE_NODISCARD bool IsButtonUp(eMouseButton mouseButton) const override;
  CE_NODISCARD bool IsButtonPressed(eMouseButton mouseButton) const override;
  CE_NODISCARD bool IsButtonReleased(eMouseButton mouseButton) const override;
private:

  int32_t m_x;
  int32_t m_y;
  int32_t m_relX;
  int32_t m_relY;
  int32_t m_wheel;
  int32_t m_wheelHorizontal;
  bool m_current[(size_t)eMouseButton::COUNT];
  bool m_last[(size_t)eMouseButton::COUNT];

  bool m_cursorVisible;
  eCursorMode m_cursorMode;
  SDL_Window *m_window;
};

}