//
// Created by Marcell on 07.05.2024.
//

#pragma once

#include <ceCore/entity/entitystate.hh>

CE_CLASS()
class ExitGameState : public CE_SUPER(ce::EntityState)
{
  CE_CLASS_GEN;
public:
  ExitGameState ();


  void Update(float tpf) override;

};
