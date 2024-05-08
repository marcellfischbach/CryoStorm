//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <ceCore/igame.hh>

class Game : public ce::iGame
{
public:
  ~Game() override = default;


  bool Initialize(ce::Engine *engine) override;

};


CE_DECLARE_GAME;