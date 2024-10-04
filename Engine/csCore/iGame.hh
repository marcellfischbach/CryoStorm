#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>

namespace cryo
{

class csEngine;

struct iGame
{
  virtual ~iGame ()  = default;

  virtual bool Initialize(cryo::csEngine* engine) = 0;
};


typedef iGame*(*create_game_instance_func_ptr)();

#define CS_DECLARE_GAME extern "C" __declspec(dllexport) cryo::iGame* create_game_instance()

#define CS_DEFINE_GAME(name) cryo::iGame* create_game_instance() \
{                                                              \
  return new name();                                           \
}

}