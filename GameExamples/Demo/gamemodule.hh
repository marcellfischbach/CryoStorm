
#pragma once

#include <ceCore/imodule.hh>



class GameModule : public ce::iModule
{
public:
  bool Register(const std::vector<std::string> &args, ce::Engine *engine) override;
  bool Initialize(const std::vector<std::string> &args, ce::Engine *engine) override;
};


CS_DECLARE_LIBRARY(ceGame);