#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/material/material.hh>
#include <ceCore/graphics/shadergraph/shadergraph.hh>

namespace ce
{


CS_CLASS()
struct CS_CORE_API iShaderGraphCompiler : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

  struct Parameters
  {
    std::string DebugName;
    bool DebugSources;
  };

  virtual Material *Compile(ShaderGraph *shaderGraph, const Parameters& parameters) = 0;

  virtual const std::string &GetError() const = 0;
};


CS_CLASS()
struct CS_CORE_API iShaderGraphCompilerFactory : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

  virtual iShaderGraphCompiler *Create() const = 0;

};

}