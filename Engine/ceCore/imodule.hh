#pragma once

#include <string>
#include <vector>

namespace ce
{

class Engine;
struct iModule
{

  virtual bool Register(const std::vector<std::string> &args, Engine *engine) = 0;
  virtual bool Initialize(const std::vector<std::string> &args, Engine *engine) = 0;

};

#define CE_DECLARE_LIBRARY(name) extern "C" __declspec(dllexport) ce::iModule* name##_load_library()
#define CE_DEFINE_LIBRARY(name) ce::iModule* name##_load_library()


}