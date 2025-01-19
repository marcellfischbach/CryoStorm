#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>

namespace cs
{

class csWorld;

struct iGame
{
  virtual ~iGame ()  = default;

  virtual bool Initialize(cs::csWorld* world) = 0;

  virtual void Shutdown (cs::csWorld* world) = 0;
};


typedef iGame*(*create_game_instance_func_ptr)();

#define CS_DECLARE_GAME extern "C" __declspec(dllexport) cs::iGame* create_game_instance()

#define CS_DEFINE_GAME(name) cs::iGame* create_game_instance() \
{                                                              \
  return new name();                                           \
}

}