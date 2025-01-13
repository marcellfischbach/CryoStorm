#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/resource/iAsset.hh>
#include <csCore/csRef.hh>
#include <exception>
#include <vector>

namespace cs
{



CS_CLASS()
class CS_CORE_API csAssetLoader : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  virtual ~csAssetLoader() = default;


  CS_NODISCARD uint32_t GetPriority() const;

  CS_NODISCARD virtual csAssetRef<iAsset> Load(const csAssetLocator & locator) const = 0;
  CS_NODISCARD bool CanLoad(const csAssetLocator & locator) const;

  CS_NODISCARD const std::vector<std::string>& GetExtensions() const;

protected:
  csAssetLoader(uint32_t priority = 0);

  void RegisterType(const std::string & extension);



private:
  uint32_t m_priority = 0;

  std::vector<std::string> m_extensions;
};

}
