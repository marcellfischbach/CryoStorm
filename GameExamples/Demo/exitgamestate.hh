//
// Created by Marcell on 07.05.2024.
//

#pragma once

#include <csCore/entity/csEntityState.hh>

CS_CLASS()
class ExitGameState : public CS_SUPER(cryo::csEntityState)
{
  CS_CLASS_GEN;
public:
  ExitGameState ();


  void Update(float tpf) override;

};
