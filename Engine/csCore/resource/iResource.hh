
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/csResourceLocator.hh>

namespace cs
{


CS_CLASS(jclass = "org.cryo.core.resource.IResource")
struct CS_CORE_API iResource : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  ~iResource() override = default;

  virtual void SetLocator(const csResourceLocator & locator) = 0;
  virtual const csResourceLocator& GetLocator() const = 0;

  virtual void Invalidate() = 0;
  virtual bool IsValid() const = 0;
};


#define CS_RESOURCE_GEN \
public: \
  void SetLocator (const csResourceLocator &locator) override \
  { \
    m_locator = locator; \
  } \
  const csResourceLocator &GetLocator () const override \
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
  csResourceLocator m_locator = csResourceLocator("");\
  bool m_valid = true

} 