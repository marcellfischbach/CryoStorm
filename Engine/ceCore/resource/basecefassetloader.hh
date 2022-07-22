//
// Created by MCEL on 07.07.2022.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/resource/iassetloader.hh>
#include <vector>

namespace ce
{


CE_CLASS()
class CE_CORE_API BaseCEFAssetLoader : public CE_SUPER(iAssetLoader)
{
  CE_CLASS_GEN_OBJECT;

protected:
  BaseCEFAssetLoader() = default;

  template<typename T>
  void AddValidFile(const std::string &fileExtension)
  {
    AddValidFile(T::GetStaticClass(), fileExtension);
  }
  void AddValidFile (const Class* cls, const std::string &fileExtension);

  virtual iObject* Load(const CrimsonFile *file, const Class* cls, const ResourceLocator& locator) const = 0;

public:
  ~BaseCEFAssetLoader() override = default;

  bool CanLoad(const Class* cls, const ResourceLocator& locator) const override ;
  CE_NODISCARD iObject* Load(const Class* cls, const ResourceLocator& locator) const override;

private:
  CE_NODISCARD static bool Open (const ResourceLocator &locator, CrimsonFile &file);


  struct ValidFile
  {
    const Class* Cls;
    std::string Extension;
  };

  std::vector<ValidFile> m_validFiles;
};

} // ce
