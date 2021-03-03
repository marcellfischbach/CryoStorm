 
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc::opengl
{

SPC_CLASS()
class GL4ShaderLoaderSpc : public SPC_SUPER(iAssetLoaderSpc)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4ShaderLoaderSpc();
  virtual ~GL4ShaderLoaderSpc() = default;


  bool CanLoad(const Class* cls, const file::File* file, const ResourceLocator* locator) const override;

  iObject* Load(const Class* cls, const file::File* file, const ResourceLocator* locator) const override;

};

SPC_CLASS()
class GL4ShaderLoader : public SPC_SUPER(iAssetLoader)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4ShaderLoader();
  virtual ~GL4ShaderLoader() = default;


  bool CanLoad(const Class * cls, const ResourceLocator & locator) const override;

  iObject* Load(const Class * cls, const ResourceLocator & locator) const override;

};


}