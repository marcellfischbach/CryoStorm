#pragma once
#include <ceOpenGL/openglexport.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace cryo::opengl
{


CS_CLASS()
class CS_OGL_API GL4ProgramLoader : public CS_SUPER(BaseCEFAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  GL4ProgramLoader();

  iObject* Load(const CrimsonFile* file, const Class* cls, const ResourceLocator& locator) const override;


};


}