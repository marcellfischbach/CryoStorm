//
// Created by MCEL on 21.03.2025.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/input/iInputSystem.hh>
#include <csCore/csRef.hh>

namespace cs
{

class csJavaKeyboard;
class csJavaMouse;

CS_CLASS(jclass="org.cryo.core.input.CsInputSystemJava")
class CS_CORE_API csJavaInputSystem : public CS_SUPER(iInputSystem)
{
  CS_CLASS_GEN_OBJECT;
public:
  csJavaInputSystem() = default;
  ~csJavaInputSystem() override = default;

  CS_FUNCTION()
  void SetKeyboard (cs::csJavaKeyboard *keyboard);

  CS_FUNCTION()
  void SetMouse (cs::csJavaMouse *mouse);

  iKeyboard *GetKeyboard() const override;
  iMouse *GetMouse() const override;

private:
  csRef<csJavaKeyboard> m_keyboard;
  csRef<csJavaMouse> m_mouse;

};

} // cs
