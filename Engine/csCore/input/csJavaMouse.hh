//
// Created by MCEL on 21.03.2025.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/input/iMouse.hh>

namespace cs
{

CS_CLASS(jclass="org.cryo.core.input.CsMouseJava")
class CS_CORE_API csJavaMouse : public CS_SUPER(iMouse)
{
  CS_CLASS_GEN_OBJECT;
public:
  csJavaMouse();
  ~csJavaMouse() override;

  void SetVisible (bool visible);
  bool IsVisible () const;

  void SetCursorMode (eCursorMode mode);
  eCursorMode GetCursorMode () const;


  int32_t GetX() const;
  int32_t GetY() const;

  int32_t GetDeltaX() const;
  int32_t GetDeltaY() const;
  int32_t GetWheel() const;
  int32_t GetWheelHorizontal() const;

  bool IsButtonDown(eMouseButton mouseButton) const;
  bool IsButtonUp(eMouseButton mouseButton) const;
  bool IsButtonPressed(eMouseButton mouseButton) const;
  bool IsButtonReleased(eMouseButton mouseButton) const;

private:
  struct csJavaMousePrivate *m_priv;
};

}
