//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <ceCore/igame.hh>

class Game : public cryo::iGame
{
public:
  ~Game() override = default;


  bool Initialize(cryo::Engine *engine) override;

};


CS_DECLARE_GAME;