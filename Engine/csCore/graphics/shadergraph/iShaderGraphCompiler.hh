#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/material/csMaterial.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>

namespace cs
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

  virtual csMaterial *Compile(csShaderGraph *shaderGraph, const Parameters& parameters) = 0;

  virtual const std::string &GetError() const = 0;
};


CS_CLASS()
struct CS_CORE_API iShaderGraphCompilerFactory : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

  virtual iShaderGraphCompiler *Create() const = 0;

};

}