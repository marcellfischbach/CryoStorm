//
// Created by MCEL on 21.03.2025.
//

#pragma once

#include <csCore/csCoreExport.hh>

#include <csCore/input/iKeyboard.hh>


namespace cs
{

CS_CLASS(jclass="org.cryo.core.input.KeyboardJava")
class CS_CORE_API csJavaKeyboard : public CS_SUPER(iKeyboard)
{
  CS_CLASS_GEN_OBJECT;
public:
  csJavaKeyboard();
  ~csJavaKeyboard() override;


  bool IsKeyDown(eKey key) const override;
  bool IsKeyUp(eKey key) const override;
  bool IsKeyPressed(eKey key) const override;
  bool IsKeyReleased(eKey key) const override;


private:
  struct csJavaKeyboardPrivate *m_priv;
};

} // cs
