//
// Created by MCEL on 08.05.2024.
//

#include <ceJavaBinding/lwjglkeyboard.hh>

namespace ce::java
{

void LwjglKeyboard::Swap()
{
  memcpy (m_prefKeys, m_keys, sizeof(m_keys));
}

void LwjglKeyboard::Update(ce::Key key, bool down)
{
  m_keys[key] = down;
}

bool LwjglKeyboard::IsKeyDown(ce::Key key) const
{
  return m_keys[key];
}

bool LwjglKeyboard::IsKeyUp(ce::Key key) const
{
  return !m_keys[key];
}


bool LwjglKeyboard::IsKeyPressed(ce::Key key) const
{
  return m_keys[key] && !m_prefKeys[key];
}

bool LwjglKeyboard::IsKeyReleased(ce::Key key) const
{
  return !m_keys[key] && m_prefKeys[key];
}


}
