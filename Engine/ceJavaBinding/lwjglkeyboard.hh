//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <ceCore/input/ikeyboard.hh>

namespace ce::java
{

class LwjglKeyboard : public iKeyboard
{
public:
  ~LwjglKeyboard() = default;

  void Swap();
  void Update (Key key, bool down);

  bool IsKeyDown(Key key) const override;
  bool IsKeyUp(Key key) const override;

  bool IsKeyPressed(Key key) const override;
  bool IsKeyReleased(Key key) const override;


private:
  bool m_keys[eK_COUNT];
  bool m_prefKeys[eK_COUNT];
};

}
