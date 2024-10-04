//
// Created by Marcell on 07.05.2024.
//

#include "exitgamestate.hh"
#include <csCore/input/csInput.hh>
#include <csCore/csEngine.hh>

ExitGameState::ExitGameState()
{
  SetNeedUpdate(true);
}

void ExitGameState::Update(float tpf)
{
  if (cryo::csInput::IsKeyPressed(cryo::eKey::eK_Escape))
  {
    cryo::csEngine::Get()->Exit(0);
  }
}
