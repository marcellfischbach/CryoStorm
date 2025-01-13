#pragma once
#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>

namespace cs::opengl
{


CS_CLASS()
class CS_OGL_API csGL4ProgramLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4ProgramLoader();

  csAssetRef<iAsset> Load(const csCryoFile *file, const csAssetLocator &locator) const override;


};


}