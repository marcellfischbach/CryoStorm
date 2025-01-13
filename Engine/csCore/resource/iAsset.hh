
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/csAssetLocator.hh>

namespace cs
{


CS_CLASS(jclass = "org.cryo.core.resource.IAsset")
struct CS_CORE_API iAsset : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  ~iAsset() override = default;

  virtual void SetLocator(const csAssetLocator & locator) = 0;
  virtual const csAssetLocator& GetLocator() const = 0;

  virtual void Invalidate() = 0;
  virtual bool IsValid() const = 0;
};


#define CS_ASSET_GEN \
public: \
  void SetLocator (const csAssetLocator &locator) override \
  { \
    m_locator = locator; \
  } \
  const csAssetLocator &GetLocator () const override \
  { \
    return m_locator; \
  } \
  void Invalidate () override \
  {\
    m_valid = false; \
  }\
  bool IsValid () const override \
  {\
    return m_valid;\
  }\
private:\
  csAssetLocator m_locator = csAssetLocator("");\
  bool m_valid = true

} 