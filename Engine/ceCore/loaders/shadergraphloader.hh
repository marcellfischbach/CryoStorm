//
// Created by Marcell on 07.07.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace ce
{

class SGNode;
class SGResourceNode;
class ShaderGraph;

CE_CLASS()
class CE_CORE_API ShaderGraphLoader : public CE_SUPER(BaseCEFAssetLoader)
{
CE_CLASS_GEN_OBJECT;
public:
  ShaderGraphLoader();
  ~ShaderGraphLoader() override = default;

  iObject *Load(const CrimsonFile *file, const Class *cls, const ResourceLocator &locator) const override;

private:
  void LoadQueue (const CrimsonFileElement *shaderGraphElement, ShaderGraph *sg) const;
  void LoadLightingMode (const CrimsonFileElement *shaderGraphElement, ShaderGraph *sg) const;

  SGNode *CreateNode(const CrimsonFileElement *nodeElement, ShaderGraph *sg) const;
  SGResourceNode *CreateResourceNode(const CrimsonFileElement *nodeElement, ShaderGraph *sg) const;
  bool LoadNodeBindingsAndValues(const CrimsonFileElement *nodeElement, SGNode *node, ShaderGraph *sg) const;
  bool LoadValue(const CrimsonFileElement *valueElement, SGNode *node, ShaderGraph *sg) const;
  bool LoadBinding(const CrimsonFileElement *valueElement, SGNode *node, ShaderGraph *sg) const;

  void LoadAttributes (const CrimsonFileElement *attributesElement, ShaderGraph *sg, const ResourceLocator &locator) const;
  void LoadAttribute (const CrimsonFileElement *attributeElement, ShaderGraph *sg, const ResourceLocator &locator) const;


};

} // ce
