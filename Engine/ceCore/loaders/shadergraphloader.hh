//
// Created by Marcell on 07.07.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace cryo
{

class csSGNode;
class csSGResourceNode;
class csShaderGraph;

CS_CLASS()
class CS_CORE_API ShaderGraphLoader : public CS_SUPER(BaseCEFAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  ShaderGraphLoader();
  ~ShaderGraphLoader() override = default;

  iObject *Load(const CrimsonFile *file, const Class *cls, const ResourceLocator &locator) const override;

private:
  void LoadQueue(const CrimsonFileElement *shaderGraphElement, csShaderGraph *sg) const;
  void LoadLightingMode(const CrimsonFileElement *shaderGraphElement, csShaderGraph *sg) const;
  void LoadBlendingMode(const CrimsonFileElement *shaderGraphElement, csShaderGraph *sg) const;

  csSGNode *CreateNode(const CrimsonFileElement *nodeElement, csShaderGraph *sg) const;
  csSGResourceNode *CreateResourceNode(const CrimsonFileElement *nodeElement, csShaderGraph *sg) const;
  bool LoadNodeBindingsAndValues(const CrimsonFileElement *nodeElement, csSGNode *node, csShaderGraph *sg) const;
  bool LoadValue(const CrimsonFileElement *valueElement, csSGNode *node, csShaderGraph *sg) const;
  bool LoadBinding(const CrimsonFileElement *valueElement, csSGNode *node, csShaderGraph *sg) const;

  void LoadAttributes(const CrimsonFileElement *attributesElement, csShaderGraph *sg, const ResourceLocator &locator) const;
  void LoadAttribute(const CrimsonFileElement *attributeElement, csShaderGraph *sg, const ResourceLocator &locator) const;


};

} // ce
