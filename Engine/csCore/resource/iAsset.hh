
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

  CS_FORCEINLINE void SetLocator(const csAssetLocator &locator)
  {
    m_locator = locator;
  }
  CS_NODISCARD CS_FORCEINLINE const csAssetLocator &GetLocator() const
  {
    return m_locator;
  }

  CS_FORCEINLINE void Invalidate()
  {
    m_valid = false;
  }
  CS_NODISCARD CS_FORCEINLINE bool IsValid() const
  {
    return m_valid;
  }
private:
  csAssetLocator m_locator = csAssetLocator("");
  bool           m_valid   = true;
};


}