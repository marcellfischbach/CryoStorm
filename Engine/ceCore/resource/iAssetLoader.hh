
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/class.hh>
#include <ceCore/csTypes.hh>
#include <ceCore/resource/csCryoFile.hh>
#include <ceCore/resource/csResourceLocator.hh>

namespace cryo
{

const int16_t DefaultPriority = 0;

CS_CLASS()
struct CS_CORE_API iAssetLoader : public iObject
{
  CS_CLASS_GEN;
  virtual ~iAssetLoader()
  {

  }

  virtual int16_t Priority() const
  {
    return DefaultPriority;
  }

  virtual bool CanLoad(const Class* cls, const csResourceLocator& locator) const = 0;

  virtual iObject* Load(const Class* cls, const csResourceLocator& locator) const = 0;

};


}