//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <csCore/input/iMouse.hh>
#include <csCore/csJava.hh>

namespace cs::java
{

CS_CLASS()
class csLwjglMouse : public iMouse
{
  CS_CLASS_GEN_OBJECT;
public:
  csLwjglMouse();
  ~csLwjglMouse() = default;

  jobject GetJObject ();

  void SetVisible (bool visible) override;
  CS_NODISCARD bool IsVisible () const override;

  void SetCursorMode (eCursorMode mode) override;
  CS_NODISCARD eCursorMode GetCursorMode () const override;


  CS_NODISCARD int32_t GetX() const override;
  CS_NODISCARD int32_t GetY() const override;

  CS_NODISCARD int32_t GetDeltaX() const override;
  CS_NODISCARD int32_t GetDeltaY() const override;
  CS_NODISCARD int32_t GetWheel() const override;
  CS_NODISCARD int32_t GetWheelHorizontal() const override;

  CS_NODISCARD bool IsButtonDown(eMouseButton mouseButton) const override;
  CS_NODISCARD bool IsButtonUp(eMouseButton mouseButton) const override;
  CS_NODISCARD bool IsButtonPressed(eMouseButton mouseButton) const override;
  CS_NODISCARD bool IsButtonReleased(eMouseButton mouseButton) const override;

private:
  jobject jobj;
  jclass cls;
};

}
