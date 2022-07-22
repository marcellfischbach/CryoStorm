#pragma once
#include <ceOpenGL/openglexport.hh>
#include <ceCore/resource/iassetloader.hh>

namespace ce::opengl
{


CE_CLASS()
class CE_OGL_API GL4ProgramLoader : public CE_SUPER(iAssetLoaderCEF)
{
  CE_CLASS_GEN_OBJECT;
public:
  GL4ProgramLoader();

  bool CanLoad(const Class* cls, const CrimsonFile* file, const ResourceLocator* locator) const override;

  iObject* Load(const Class* cls, const CrimsonFile* file, const ResourceLocator* locator) const override;


};


}