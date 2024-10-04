//
// Created by MCEL on 07.07.2022.
//

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/csClass.hh>
#include <ceCore/resource/iAssetLoader.hh>
#include <vector>

namespace cryo
{


CS_CLASS()
class CS_CORE_API csBaseCSFAssetLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;

protected:
  csBaseCSFAssetLoader() = default;

  template<typename T>
  void AddValidFile(const std::string &fileExtension)
  {
    AddValidFile(T::GetStaticClass(), fileExtension);
  }
  void AddValidFile(const csClass* cls, const std::string &fileExtension);

  virtual iObject* Load(const csCryoFile *file, const csClass* cls, const csResourceLocator& locator) const = 0;

public:
  ~csBaseCSFAssetLoader() override = default;

  bool CanLoad(const csClass* cls, const csResourceLocator& locator) const override ;
  CS_NODISCARD iObject* Load(const csClass* cls, const csResourceLocator& locator) const override;

private:
  CS_NODISCARD static bool Open(const csResourceLocator &locator, csCryoFile &file);


  struct ValidFile
  {
    const csClass * Cls;
    std::string Extension;
  };

  std::vector<ValidFile> m_validFiles;
};

} // ce
