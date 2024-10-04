#pragma once
#include <ceOpenGL/openglexport.hh>
#include <ceCore/resource/csBaseCSFAssetLoader.hh>

namespace cryo::opengl
{


CS_CLASS()
class CS_OGL_API GL4ProgramLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  GL4ProgramLoader();

  iObject* Load(const csCryoFile* file, const csClass* cls, const csResourceLocator& locator) const override;


};


}