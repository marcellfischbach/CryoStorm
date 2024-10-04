//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <csCore/iGame.hh>

class Game : public cryo::iGame
{
public:
  ~Game() override = default;


  bool Initialize(cryo::csEngine *engine) override;

};


CS_DECLARE_GAME;