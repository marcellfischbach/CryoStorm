#pragma once


#include <spcCore/input/imouse.hh>
#include <SDL.h>

namespace spc
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
  SPC_NODISCARD  bool IsVisible () const override;
  void SetCursorMode (eCursorMode mode) override;
  SPC_NODISCARD eCursorMode GetCursorMode () const override;


  SPC_NODISCARD int32_t GetX() const override;
  SPC_NODISCARD int32_t GetY() const override;

  SPC_NODISCARD int32_t GetDeltaX() const override;
  SPC_NODISCARD int32_t GetDeltaY() const override;
  SPC_NODISCARD int32_t GetWheel() const override;
  SPC_NODISCARD int32_t GetWheelHorizontal() const override;


  SPC_NODISCARD bool IsButtonDown(MouseButton mouseButton) const override;
  SPC_NODISCARD bool IsButtonUp(MouseButton mouseButton) const override;
  SPC_NODISCARD bool IsButtonPressed(MouseButton mouseButton) const override;
  SPC_NODISCARD bool IsButtonReleased(MouseButton mouseButton) const override;
private:

  int32_t m_x;
  int32_t m_y;
  int32_t m_relX;
  int32_t m_relY;
  int32_t m_wheel;
  int32_t m_wheelHorizontal;
  bool m_current[eMB_COUNT];
  bool m_last[eMB_COUNT];

  bool m_cursorVisible;
  eCursorMode m_cursorMode;
  SDL_Window *m_window;
};

}