//
// Created by Marcell on 02.01.2025.
//

#pragma once

#include <csCore/resource/csResourceLocator.hh>
#include <vector>
#include <string>

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

  void Save(const cs::csResourceLocator &locator);
private:
  void CollectNodes ();
  std::string GetNodeSource (cs::csSGNode *node);
  std::string GetAttributeSource (cs::csSGNode *node);
  std::string GetShaderGraphSource ();

  cs::csShaderGraph* m_shaderGraph;
  std::vector<cs::csSGNode *> m_sortedNodes;
};
