 
#pragma once

#include <SpiceOpenGL/openglexport.hh>
#include <SpiceCore/graphics/shading/ishaderloader.hh>

namespace Spice
{

SPICE_CLASS()
class GL4ShaderLoader : public SPICE_SUPER(iShaderLoader)
{
  SPICE_CLASS_GEN_OBJECT;
public:
  GL4ShaderLoader();
  virtual ~GL4ShaderLoader()
  {

  }


  virtual iShader* Load(const ResourceLocator & locator);

};



}