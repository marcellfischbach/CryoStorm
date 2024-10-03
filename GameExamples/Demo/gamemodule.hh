
#pragma once

#include <ceCore/imodule.hh>



class GameModule : public cryo::iModule
{
public:
  bool Register(const std::vector<std::string> &args, cryo::Engine *engine) override;
  bool Initialize(const std::vector<std::string> &args, cryo::Engine *engine) override;
};


CS_DECLARE_LIBRARY(ceGame);