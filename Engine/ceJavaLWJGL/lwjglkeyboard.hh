//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <ceCore/input/iKeyboard.hh>


namespace cryo::java
{

class LwjglKeyboard : public iKeyboard
{
public:
  LwjglKeyboard();
  ~LwjglKeyboard() = default;

  jobject GetJObject ();


  bool IsKeyDown(eKey key) const override;
  bool IsKeyUp(eKey key) const override;

  bool IsKeyPressed(eKey key) const override;
  bool IsKeyReleased(eKey key) const override;


private:
  jobject jobj;
  jclass cls;
};

}
