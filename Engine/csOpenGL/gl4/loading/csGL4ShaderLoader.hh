 
#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/resource/iAssetLoader.hh>

namespace cs::opengl
{


CS_CLASS()
class csGL4ShaderLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4ShaderLoader();
  virtual ~csGL4ShaderLoader() = default;


  bool CanLoad(const csClass * cls, const csResourceLocator & locator) const override;

  iObject* Load(const csClass * cls, const csResourceLocator & locator) const override;

};


}