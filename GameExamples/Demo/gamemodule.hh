
#pragma once

#include <csCore/iModule.hh>



class GameModule : public cs::iModule
{
public:
  bool Register(const std::vector<std::string> &args, cs::csEngine *engine) override;
  bool Initialize(const std::vector<std::string> &args, cs::csEngine *engine) override;
};


CS_DECLARE_LIBRARY(csGame);