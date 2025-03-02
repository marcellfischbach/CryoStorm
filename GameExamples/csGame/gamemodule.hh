
#pragma once

#include <csCore/iModule.hh>


CS_CLASS()
class GameModule : public CS_SUPER(cs::iModule)
{
CS_CLASS_GEN_OBJECT;
public:
  bool Register(const std::vector<std::string> &args, cs::csEngine *engine) override;
  bool Initialize(const std::vector<std::string> &args, cs::csEngine *engine) override;
  void Shutdown (const std::vector<std::string> &args, cs::csEngine *engine) override;
};


CS_DECLARE_LIBRARY(csGame);