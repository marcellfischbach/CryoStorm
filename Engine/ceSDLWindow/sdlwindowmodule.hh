//
// Created by mcel on 30.04.2024.
//

#pragma once

#include <ceSDLWindow/sdlwindowexport.hh>
#include <ceCore/imodule.hh>

namespace ce::sdlwindow
{

class CE_SDL_API SDLWindowModule : public iModule
{
public:

  bool Register(int argc, char** argv, Engine* engine) override;
  bool Initialize(int argc, char** argv, Engine* engine) override;

};

}
