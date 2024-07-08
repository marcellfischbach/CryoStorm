//
// Created by Marcell on 07.07.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/iassetloader.hh>

namespace ce
{

class SGNode;
class ShaderGraph;

CE_CLASS()
class CE_CORE_API ShaderGraphLoader : public CE_SUPER(iAssetLoader)
{
CE_CLASS_GEN_OBJECT;
public:
  ShaderGraphLoader() = default;
  ~ShaderGraphLoader() override = default;

  bool CanLoad(const Class* cls, const ResourceLocator& locator) const override;

  iObject* Load(const Class* cls, const ResourceLocator& locator) const override;

private:
  void LoadNode (const CrimsonFileElement & nodeElement, ShaderGraph *shaderGraph, std::map<std::string, SGNode*> &nodes);

};

} // ce
