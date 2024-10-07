//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <csCore/iGame.hh>

class Game : public cs::iGame
{
public:
  ~Game() override = default;


  bool Initialize(cs::csEngine *engine) override;

};


CS_DECLARE_GAME;