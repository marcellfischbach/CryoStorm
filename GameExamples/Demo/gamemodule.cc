//

#include "gamemodule.hh"
#include "master.refl.cc"

CS_DEFINE_LIBRARY(csGame)
{
  return new GameModule();
}


bool GameModule::Register(const std::vector<std::string> &args, cs::csEngine *engine)
{
  register_classes();
  return true;
}

bool GameModule::Initialize(const std::vector<std::string> &args, cs::csEngine *engine)
{
  return true;
}

void GameModule::Shutdown(const std::vector<std::string> &args, cs::csEngine *engine)
{

}
