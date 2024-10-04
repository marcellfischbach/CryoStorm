//
// Created by Marcell on 01.07.2024.
//

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/class.hh>
#include <ceCore/resource/iAssetLoader.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API csTextFileLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csTextFileLoader() = default;
  ~csTextFileLoader() override = default;

  bool CanLoad(const Class *cls, const csResourceLocator &locator) const override;

  iObject *Load(const Class *cls, const csResourceLocator &locator) const override;

};

} // ce