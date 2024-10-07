//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <csCore/input/iKeyboard.hh>


namespace cs::java
{

class csLwjglKeyboard : public iKeyboard
{
public:
  csLwjglKeyboard();
  ~csLwjglKeyboard() = default;

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
