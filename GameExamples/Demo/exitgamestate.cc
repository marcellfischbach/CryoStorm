//
// Created by Marcell on 07.05.2024.
//

#include "exitgamestate.hh"
#include <ceCore/input/input.hh>
#include <ceCore/engine.hh>

ExitGameState::ExitGameState()
{
  SetNeedUpdate(true);
}

void ExitGameState::Update(float tpf)
{
  if (ce::Input::IsKeyPressed(ce::Key::eK_Escape))
  {
    ce::Engine::Get()->Exit(0);
  }
}
