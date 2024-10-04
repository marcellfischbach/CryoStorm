
#pragma once

#include <ceCore/iModule.hh>



class GameModule : public cryo::iModule
{
public:
  bool Register(const std::vector<std::string> &args, cryo::csEngine *engine) override;
  bool Initialize(const std::vector<std::string> &args, cryo::csEngine *engine) override;
};


CS_DECLARE_LIBRARY(ceGame);