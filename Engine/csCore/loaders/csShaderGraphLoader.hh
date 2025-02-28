//
// Created by Marcell on 07.07.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>

namespace cs
{

class csSGNode;
class csSGResourceNode;
class csShaderGraph;

CS_CLASS()
class CS_CORE_API csShaderGraphLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csShaderGraphLoader();
  ~csShaderGraphLoader() override = default;

  csOwned<iAsset> Load(const file::csCryoFile *file, const csAssetLocator &locator) const override;

private:
  void LoadQueue(const file::csCryoFileElement * shaderGraphElement, csShaderGraph * sg) const;
  void LoadLightingMode(const file::csCryoFileElement * shaderGraphElement, csShaderGraph * sg) const;
  void LoadBlendingMode(const file::csCryoFileElement * shaderGraphElement, csShaderGraph * sg) const;

  csOwned<csSGNode> CreateNode(const file::csCryoFileElement * nodeElement, csShaderGraph * sg) const;
  csOwned<csSGResourceNode> CreateResourceNode(const file::csCryoFileElement * nodeElement, csShaderGraph * sg) const;
  bool LoadResourceDefaults(const file::csCryoFileElement * nodeElement, csSGResourceNode * resourceNode, csShaderGraph * sg) const;
  bool LoadNodeBindingsAndValues(const file::csCryoFileElement * nodeElement, csSGNode * node, csShaderGraph * sg) const;
  bool LoadValue(const file::csCryoFileElement * valueElement, csSGNode * node, csShaderGraph * sg) const;
  bool LoadBinding(const file::csCryoFileElement * valueElement, csSGNode * node, csShaderGraph * sg) const;


  void LoadNodePositions(const file::csCryoFileElement * valueElement, csSGNode * node) const;

};

} // ce
