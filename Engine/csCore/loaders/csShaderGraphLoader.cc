//
// Created by Marcell on 07.07.2024.
//

#include <csCore/loaders/csShaderGraphLoader.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/graphics/eBlendFactor.hh>
#include <csCore/graphics/material/csMaterial.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <csCore/graphics/shadergraph/iShaderGraphCompiler.hh>
#include <csCore/csClassRegistry.hh>
#include <csCore/resource/csAssetManager.hh>

namespace cs
{

csShaderGraphLoader::csShaderGraphLoader()
  : csBaseCSFAssetLoader()
{
  AddValidFile<csShaderGraph>("SG");
  AddValidFile<csShaderGraph>("SHADERGRAPH");

  AddValidFile<csMaterial>("SG");
  AddValidFile<csMaterial>("SHADERGRAPH");
}


iObject* csShaderGraphLoader::Load(const csCryoFile* file, const csClass* cls, const csResourceLocator& locator) const
{
  const csCryoFileElement* root = file->Root();
  const csCryoFileElement* shaderGraphElement = root->GetChild("shaderGraph");
  auto sg = new csShaderGraph();
  csAutoRelease autoRelSG(sg);


  if (!shaderGraphElement)
  {
    fprintf(stderr, "Invalid shader graph. Root element must be 'shaderGraph'\n");
    return nullptr;
  }

  auto receiveShadowElement = shaderGraphElement->GetChild("receiveShadow");
  if (receiveShadowElement)
  {
    sg->SetReceiveShadow(receiveShadowElement->GetAttribute(0, 1));
  }

  LoadQueue(shaderGraphElement, sg);
  LoadLightingMode(shaderGraphElement, sg);
  LoadBlendingMode(shaderGraphElement, sg);


  const csCryoFileElement* nodesElements = shaderGraphElement->GetChild("nodes");
  if (!nodesElements)
  {
    fprintf(stderr, "Invalid shader graph. No 'nodes' element\n");
    return nullptr;
  }

  for (size_t i = 0, in = nodesElements->GetNumberOfChildren(); i < in; ++i)
  {
    const csCryoFileElement* childElement = nodesElements->GetChild(i);
    if (!childElement)
    {
      continue;
    }

    csSGNode* node = nullptr;
    const std::string& tagName = childElement->GetTagName();
    if (tagName == "node")
    {
      node = CreateNode(childElement, sg);
    }
    else if (tagName == "resource")
    {
      node = CreateResourceNode(childElement, sg);
    }
    else if (tagName == "shaderGraph")
    {
      node = sg;
    }

    if (!node)
    {
      return nullptr;
    }

    LoadNodePositions(childElement, node);

    if (!LoadNodeBindingsAndValues(childElement, node, sg))
    {
      return nullptr;
    }

  }


  if (cls->IsInstanceOf<csMaterial>())
  {
    // Now compile the shader graph to a material

    auto compilerFactory = cs::csObjectRegistry::Get<cs::iShaderGraphCompilerFactory>();
    if (compilerFactory)
    {
      auto compiler = compilerFactory->Create();
      if (compiler)
      {
        iShaderGraphCompiler::Parameters parameters{};
        memset(&parameters, 0, sizeof(parameters));
        parameters.DebugName = locator.Encoded();
        parameters.DebugSources = true;

        auto shader = compiler->Compile(sg, parameters);
        if (!shader)
        {
          printf("Unable to compiler '%s'\n", locator.Encoded().c_str());
          printf("%s\n", compiler->GetError().c_str());
        }

        return shader;
      }
    }
  }
  else if (cls->IsInstanceOf<csShaderGraph>())
  {
    autoRelSG.Clear(); // keep the sg alive
    return sg;
  }

  return nullptr;
}

void csShaderGraphLoader::LoadQueue(const cs::csCryoFileElement* shaderGraphElement, csShaderGraph* sg) const
{
  auto queueElement = shaderGraphElement->GetChild("queue");
  if (queueElement)
  {
    eRenderQueue queue = eRenderQueue::Default;
    std::string queueString = queueElement->GetAttribute(0, "Default");
    if (queueString == "Transparency")
    {
      queue = eRenderQueue::Transparency;
    }
    sg->SetQueue(queue);
  }
}

void csShaderGraphLoader::LoadLightingMode(const csCryoFileElement* shaderGraphElement, csShaderGraph* sg) const
{
  auto lightingElement = shaderGraphElement->GetChild("lighting");
  if (lightingElement)
  {
    csShaderGraph::eLightingMode lighting = csShaderGraph::eLM_Default;
    std::string lightingString = lightingElement->GetAttribute(0, "Default");
    if (lightingString == "Attenuated")
    {
      lighting = csShaderGraph::eLM_Attenuated;
    }
    else if (lightingString == "Unlit")
    {
      lighting = csShaderGraph::eLM_Unlit;
    }
    sg->SetLightingMode(lighting);
  }
}

void csShaderGraphLoader::LoadBlendingMode(const csCryoFileElement* shaderGraphElement, csShaderGraph* sg) const
{
  auto blendingElement = shaderGraphElement->GetChild("blending");
  if (blendingElement)
  {
    csShaderGraph::eBlendingMode blending = csShaderGraph::eBM_Off;
    std::string blendingString = blendingElement->GetAttribute(0, "Off");
    if (blendingString == "Alpha")
    {
      blending = csShaderGraph::eBM_Alpha;
    }
    else if (blendingString == "Add")
    {
      blending = csShaderGraph::eBM_Add;
    }
    sg->SetBlendingMode(blending);
  }
}


csSGNode* csShaderGraphLoader::CreateNode(const cs::csCryoFileElement* nodeElement, cs::csShaderGraph* sg) const
{
  const std::string& nodeTypeName = nodeElement->GetAttribute(0, "");
  if (nodeTypeName.empty())
  {
    return nullptr;
  }
  const csClass* nodeTypeClass = csClassRegistry::Get()->GetClass(nodeTypeName);
  if (!nodeTypeClass)
  {
    fprintf(stderr, "Unable to loader shader graph: Class name '%s' node found.\n", nodeTypeName.c_str());
    return nullptr;
  }

  const std::string& nodeKey = nodeElement->GetAttribute(1, "");
  if (nodeKey.empty())
  {
    fprintf(stderr, "Unable to create node '%s'. No key defined.\n", nodeTypeName.c_str());
    return nullptr;
  }

  return sg->Add(nodeTypeClass, nodeKey);
}


csSGResourceNode*
csShaderGraphLoader::CreateResourceNode(const cs::csCryoFileElement* nodeElement, cs::csShaderGraph* sg) const
{
  const std::string& nodeTypeName = nodeElement->GetAttribute(0, "");
  if (nodeTypeName.empty())
  {
    return nullptr;
  }
  const csClass* nodeTypeClass = csClassRegistry::Get()->GetClass(nodeTypeName);
  if (!nodeTypeClass)
  {
    fprintf(stderr, "Unable to loader shader graph: Class name '%s' node found.\n", nodeTypeName.c_str());
    return nullptr;
  }

  const std::string& nodeKey = nodeElement->GetAttribute(1, "");
  if (nodeKey.empty())
  {
    fprintf(stderr, "Unable to create node '%s'. No key defined.\n", nodeTypeName.c_str());
    return nullptr;
  }

  const std::string& nodeResourceName = nodeElement->GetAttribute(2, "");
  if (nodeResourceName.empty())
  {
    fprintf(stderr,
      "Unable to create node '%s'#'%s'. No resource name defined.\n",
      nodeTypeName.c_str(),
      nodeKey.c_str());
    return nullptr;
  }

  csSGResourceNode* node = sg->AddResource(nodeTypeClass, nodeKey, nodeResourceName);
  if (!node)
  {
    fprintf(stderr, "Unable to create node '%s'.\n", nodeTypeName.c_str());
    return nullptr;
  }

  if (!LoadResourceDefaults(nodeElement, node, sg))
  {
    CS_RELEASE(node);
    return nullptr;
  }


  return node;
}


bool csShaderGraphLoader::LoadResourceDefaults(const csCryoFileElement* nodeElement, csSGResourceNode* resourceNode, csShaderGraph* sg) const
{
  for (size_t i = 0, in = nodeElement->GetNumberOfChildren(); i < in; i++)
  {
    const csCryoFileElement* childElement = nodeElement->GetChild(i);
    if (!childElement)
    {
      continue;
    }
    const std::string& tagName = childElement->GetTagName();
    std::array<float, 16> floats;
    std::array<int, 4> ints;
    csResourceLocator locator("");
    if (tagName == "defaultFloat")
    {
      size_t num = childElement->GetAttribute(0, 0);
      num = num > 16 ? 16 : num;
      for (size_t j = 0; j < num; j++)
      {
        floats[j] = childElement->GetAttribute(j + 1, 0.0f);
      }
    }
    else if (tagName == "defaultInt")
    {
      size_t num = childElement->GetAttribute(0, 0);
      num = num > 4 ? 4 : num;
      for (size_t j = 0; j < num; j++)
      {
        ints[j] = childElement->GetAttribute(j + 1, 0);
      }
    }
    else if (tagName == "defaultLocator")
    {
      locator = csResourceLocator(childElement->GetAttribute(0, ""));
    }

    resourceNode->SetDefault(floats);
    resourceNode->SetDefault(ints);
    resourceNode->SetDefault(locator);

  }
  return true;
}

bool csShaderGraphLoader::LoadNodeBindingsAndValues(const csCryoFileElement* nodeElement,
  csSGNode* node,
  csShaderGraph* sg) const
{
  for (size_t i = 0, in = nodeElement->GetNumberOfChildren(); i < in; i++)
  {
    const csCryoFileElement* childElement = nodeElement->GetChild(i);
    if (!childElement)
    {
      continue;
    }

    const std::string& tagName = childElement->GetTagName();
    if (tagName == "value")
    {
      if (!LoadValue(childElement, node, sg))
      {
        return false;
      }
    }
    else if (tagName == "binding")
    {
      if (!LoadBinding(childElement, node, sg))
      {
        return false;
      }
    }
  }
  return true;
}

bool csShaderGraphLoader::LoadValue(const csCryoFileElement* valueElement,
  csSGNode* node,
  csShaderGraph* sg) const
{
  size_t idx = valueElement->GetAttribute(0, 0xffff);
  if (idx == 0xffff)
  {
    fprintf(stderr, "Invalid value index of '%s'\n", node->GetKey().c_str());
    return false;
  }


  csSGNodeInput* input = node->GetInput(idx);
  if (!input)
  {
    fprintf(stderr, "The index %zu is no valid input index of '%s'\n", idx, node->GetKey().c_str());
    return false;
  }

  float value = valueElement->GetAttribute(1, 0.0f);
  input->SetScalar(value);
  return true;
}


static bool is_uint(const std::string& str)
{
  if (str.empty())
  {
    return false;
  }
  for (auto ch : str)
  {
    if (ch < '0' || ch > '9')
    {
      return false;
    }
  }
  return true;
}

bool csShaderGraphLoader::LoadBinding(const csCryoFileElement* valueElement,
  csSGNode* node,
  csShaderGraph* sg) const
{
  size_t idx;
  std::string idxName = valueElement->GetAttribute(0, "");
  if (is_uint(idxName))
  {
    idx = atoi(idxName.c_str());
  }
  else
  {
    int idxNameIdx = node->IndexOfInput(idxName);
    if (idxNameIdx == -1)
    {
      idx = 0xffff;
    }
    else
    {
      idx = idxNameIdx;
    }
  }


  csSGNodeInput* input = node->GetInput(idx);
  if (!input)
  {
    fprintf(stderr, "The index '%s' is no valid input index of '%s'\n", idxName.c_str(), node->GetKey().c_str());
    return false;
  }

  std::string bindingInputKey = valueElement->GetAttribute(1, "");
  if (bindingInputKey.empty())
  {
    fprintf(stderr, "No binding input key is defined for '%s'\n", node->GetKey().c_str());
    return false;
  }

  csSGNode* bindingInputNode = sg->GetNode(bindingInputKey);
  if (!bindingInputNode)
  {
    fprintf(stderr,
      "Binding input '%s' could not be found in shader graph for '%s'\n",
      bindingInputKey.c_str(),
      node->GetKey().c_str());
    return false;
  }


  size_t bindingInputOutputIdx = 0;
  idxName = valueElement->GetAttribute(2, "0");
  if (is_uint(idxName))
  {
    bindingInputOutputIdx = atoi(idxName.c_str());
  }
  else
  {
    int idxNameIdx = bindingInputNode->IndexOfOutput(idxName);
    if (idxNameIdx == -1)
    {
      bindingInputOutputIdx = 0xffff;
    }
    else
    {
      bindingInputOutputIdx = idxNameIdx;
    }
  }

  csSGNodeOutput* output = bindingInputNode->GetOutput(bindingInputOutputIdx);
  if (!output)
  {
    fprintf(stderr,
      "The index '%s' is no valid output index of '%s'\n",
      idxName.c_str(),
      bindingInputNode->GetKey().c_str());
    return false;
  }


  node->Bind(idx, bindingInputNode, bindingInputOutputIdx);
  return true;
}



void csShaderGraphLoader::LoadNodePositions(const csCryoFileElement* nodeElement, csSGNode* node) const
{
  float posX = nodeElement->GetAttribute("x", 0.0f);
  float posY = nodeElement->GetAttribute("y", 0.0f);
  node->SetPosition(csVector2f(posX, posY));

}

} // ce