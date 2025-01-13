 
#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/resource/csAssetLoader.hh>

namespace cs::opengl
{


CS_CLASS()
class csGL4ShaderLoader : public CS_SUPER(csAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4ShaderLoader();
  virtual ~csGL4ShaderLoader() = default;



  csAssetRef<iAsset> Load(const csAssetLocator & locator) const override;

};


}