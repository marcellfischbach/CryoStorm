//
// Created by MCEL on 08.05.2024.
//

#include <ceJavaBinding/lwjglkeyboard.hh>

namespace ce::java
{

bool LwjglKeyboard::IsKeyDown(ce::Key key) const
{
  return false;
}

bool LwjglKeyboard::IsKeyUp(ce::Key key) const
{
  return true;
}


bool LwjglKeyboard::IsKeyPressed(ce::Key key) const
{
  return false;
}

bool LwjglKeyboard::IsKeyReleased(ce::Key key) const
{
  return false;
}


}
