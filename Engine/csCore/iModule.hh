#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <string>
#include <vector>

namespace cs
{

class csEngine;
CS_CLASS()
struct CS_CORE_API iModule : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iModule () override  = default;

  virtual bool Register(const std::vector<std::string> &args, csEngine *engine) = 0;
  virtual bool Initialize(const std::vector<std::string> &args, csEngine *engine) = 0;
  virtual void Shutdown (const std::vector<std::string> &args, csEngine *engine) = 0;

};

#define CS_DECLARE_LIBRARY(name) extern "C" __declspec(dllexport) cs::iModule* name##_load_library()
#define CS_DEFINE_LIBRARY(name) cs::iModule* name##_load_library()


}