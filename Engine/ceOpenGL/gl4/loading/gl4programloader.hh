#pragma once
#include <ceOpenGL/openglexport.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace ce::opengl
{


CE_CLASS()
class CE_OGL_API GL4ProgramLoader : public CE_SUPER(BaseCEFAssetLoader)
{
  CE_CLASS_GEN_OBJECT;
public:
  GL4ProgramLoader();

  iObject* Load(const CrimsonFile* file, const Class* cls, const ResourceLocator& locator) const override;


};


}