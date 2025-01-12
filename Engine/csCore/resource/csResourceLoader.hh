//
// Created by Marcell on 12.01.2025.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/iResource.hh>
#include <csCore/resource/csResourceLocator.hh>

#include <vector>
#include <string>

namespace cs
{

CS_CLASS()
class CS_CORE_API csResourceLoader : public CS_SUPER(iObject)
{
  uint32_t GetPriority () const;

  virtual bool IsValidFor(const csResourceLocator &locator) const;
  virtual iResource* Load (const csResourceLocator &locator) = 0;

protected:
  csResourceLoader (uint32_t priority);
  ~csResourceLoader() override;

  void AddExtension (const std::string &extension);


private:
  uint32_t m_priority;
  std::vector<std::string> m_extensions;
};

} // cs
