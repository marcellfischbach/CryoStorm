//
// Created by Marcell on 01.07.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/iAssetLoader.hh>

namespace cs
{


CS_CLASS()
class CS_CORE_API csTextFileLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csTextFileLoader() = default;
  ~csTextFileLoader() override = default;

  bool CanLoad(const csClass *cls, const csResourceLocator &locator) const override;

  iObject *Load(const csClass *cls, const csResourceLocator &locator) const override;

};

} // ce
