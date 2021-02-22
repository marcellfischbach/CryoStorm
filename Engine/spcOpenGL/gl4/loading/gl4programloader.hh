#pragma once
#include <spcOpenGL/openglexport.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc::opengl
{


SPC_CLASS()
class SPC_OGL_API GL4ProgramLoader : public SPC_SUPER(iAssetLoaderSpc)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4ProgramLoader();

  bool CanLoad(const Class* cls, const file::File* file, const ResourceLocator* locator) const override;

  iObject* Load(const Class* cls, const file::File* file, const ResourceLocator* locator) const override;


};


}