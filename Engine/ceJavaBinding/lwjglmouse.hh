//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <ceCore/input/imouse.hh>
#include <ceCore/java.hh>

namespace ce::java
{

class LwjglMouse : public iMouse
{
public:
  LwjglMouse();
  ~LwjglMouse() = default;

  void SetVisible (bool visible) override;
  CE_NODISCARD bool IsVisible () const override;

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
  jobject jobj;
  jclass cls;
};

}
