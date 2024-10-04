 
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/resource/iAssetLoader.hh>

namespace cryo::opengl
{


CS_CLASS()
class GL4ShaderLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  GL4ShaderLoader();
  virtual ~GL4ShaderLoader() = default;


  bool CanLoad(const csClass * cls, const csResourceLocator & locator) const override;

  iObject* Load(const csClass * cls, const csResourceLocator & locator) const override;

};


}