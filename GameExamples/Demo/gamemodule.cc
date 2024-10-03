//

#include "gamemodule.hh"
#include "master.refl.cc"

CS_DEFINE_LIBRARY(ceGame)
{
  return new GameModule();
}


bool GameModule::Register(const std::vector<std::string> &args, ce::Engine *engine)
{
  register_classes();
  return true;
}

bool GameModule::Initialize(const std::vector<std::string> &args, ce::Engine *engine)
{
  return true;
}
