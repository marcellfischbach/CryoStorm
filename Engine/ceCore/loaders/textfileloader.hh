//
// Created by Marcell on 01.07.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/resource/iassetloader.hh>

namespace ce
{


CE_CLASS()
class CE_CORE_API TextFileLoader : public CE_SUPER(iAssetLoader)
{
  CE_CLASS_GEN_OBJECT;
public:
  TextFileLoader() = default;
  ~TextFileLoader() override = default;

  bool CanLoad(const Class *cls, const ResourceLocator &locator) const override;

  iObject *Load(const Class *cls, const ResourceLocator &locator) const override;

};

} // ce
