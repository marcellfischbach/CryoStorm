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
  if (cs::csInput::IsKeyPressed(cs::eKey::eK_Escape))
  {
    cs::csEngine::Get()->Exit(0);
  }
}
