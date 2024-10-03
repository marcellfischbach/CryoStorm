//
// Created by Marcell on 01.07.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/resource/iassetloader.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API TextFileLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  TextFileLoader() = default;
  ~TextFileLoader() override = default;

  bool CanLoad(const Class *cls, const ResourceLocator &locator) const override;

  iObject *Load(const Class *cls, const ResourceLocator &locator) const override;

};

} // ce
