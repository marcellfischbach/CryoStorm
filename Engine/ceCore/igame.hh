#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>

namespace ce
{

class Engine;

struct iGame
{
  virtual ~iGame ()  = default;

  virtual bool Initialize (ce::Engine* engine) = 0;
};


typedef iGame*(*create_game_instance_func_ptr)();

#define CE_DECLARE_GAME extern "C" __declspec(dllexport) ce::iGame* create_game_instance()

#define CE_DEFINE_GAME(name) ce::iGame* create_game_instance() \
{                                                              \
  return new name();                                           \
}

}