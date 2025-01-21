//
// Created by Marcell on 02.01.2025.
//

#pragma once

#include <csCore/resource/csAssetLocator.hh>
#include <vector>
#include <string>
#include <csCore/csRef.hh>

namespace cs
{
class csShaderGraph;
class csSGNode;
}

class ShaderGraphSaver
{
public:
  ShaderGraphSaver(cs::csShaderGraph* shaderGraph);
  ~ShaderGraphSaver() = default;

  void Save(const cs::csAssetLocator &locator);
private:
  void CollectNodes ();
  std::string GetNodeSource (cs::csSGNode *node);
  std::string GetAttributeSource (cs::csSGNode *node);
  std::string GetShaderGraphSource ();

  cs::csRef<cs::csShaderGraph> m_shaderGraph;
  std::vector<cs::csRef<cs::csSGNode>> m_sortedNodes;
};
