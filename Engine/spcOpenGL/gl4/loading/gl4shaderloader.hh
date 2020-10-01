 
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/shading/ishaderloader.hh>

namespace spc
{

SPC_CLASS()
class GL4ShaderLoader : public SPC_SUPER(iShaderLoader)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4ShaderLoader();
  virtual ~GL4ShaderLoader()
  {

  }


  virtual iShader* Load(const ResourceLocator & locator);

};



}