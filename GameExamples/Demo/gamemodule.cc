//

#include "gamemodule.hh"
#include "master.refl.cc"

CE_DEFINE_LIBRARY(ceGame)
{
  return new GameModule();
}


bool GameModule::Register(int argc, char **argv, ce::Engine *engine)
{
  register_classes();
  return true;
}

bool GameModule::Initialize(int argc, char **argv, ce::Engine *engine)
{
  return true;
}
