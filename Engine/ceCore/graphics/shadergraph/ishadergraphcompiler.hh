#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/material/material.hh>
#include <ceCore/graphics/shadergraph/shadergraph.hh>

namespace ce
{


CE_CLASS()
struct CE_CORE_API iShaderGraphCompiler : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;

  virtual Material *Compile (ShaderGraph* shaderGraph) = 0;

  virtual const std::string& GetError() const = 0;
};


CE_CLASS()
struct CE_CORE_API iShaderGraphCompilerFactory : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;

  virtual iShaderGraphCompiler* Create() const = 0;

};

}