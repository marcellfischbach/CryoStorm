#pragma once

#include <string>
#include <vector>

namespace cs
{

class csEngine;
struct iModule
{

  virtual bool Register(const std::vector<std::string> &args, csEngine *engine) = 0;
  virtual bool Initialize(const std::vector<std::string> &args, csEngine *engine) = 0;

};

#define CS_DECLARE_LIBRARY(name) extern "C" __declspec(dllexport) cs::iModule* name##_load_library()
#define CS_DEFINE_LIBRARY(name) cs::iModule* name##_load_library()


}