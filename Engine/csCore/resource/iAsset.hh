
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/csAssetLocator.hh>

namespace cs
{


CS_CLASS(Virtual, jclass = "org.cryo.core.resource.IAsset")
struct CS_CORE_API iAsset : public CS_SUPER(iObject)
{
CS_CLASS_GEN;

  ~iAsset() override = default;

  CS_FORCEINLINE virtual void SetLocator(const csAssetLocator &locator)
  {
    m_locator = locator;
  }
  CS_NODISCARD CS_FORCEINLINE const csAssetLocator &GetLocator() const
  {
    return m_locator;
  }

  CS_FORCEINLINE virtual void Invalidate()
  {
    m_valid = false;
  }
  CS_FORCEINLINE virtual void Revalidate()
  {
    m_valid = true;
  }
  CS_NODISCARD CS_FORCEINLINE bool IsValid() const
  {
    return m_valid;
  }

  CS_FORCEINLINE void GetSubAssetName(const std::string &subAssetName)
  {
    m_subAssetName = subAssetName;
  }

  CS_NODISCARD CS_FORCEINLINE const std::string &GetSubAssetName() const
  {
    return m_subAssetName;
  }

  CS_NODISCARD virtual iAsset* GetSubAsset(const std::string &subAssetName) const
  {
    return nullptr;
  }

private:
  csAssetLocator m_locator = csAssetLocator("");
  std::string    m_subAssetName;
  bool           m_valid   = true;
};


}