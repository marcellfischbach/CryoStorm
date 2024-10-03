//
// Created by Marcell on 07.05.2024.
//

#pragma once

#include <ceCore/entity/entitystate.hh>

CS_CLASS()
class ExitGameState : public CS_SUPER(ce::EntityState)
{
  CS_CLASS_GEN;
public:
  ExitGameState ();


  void Update(float tpf) override;

};
