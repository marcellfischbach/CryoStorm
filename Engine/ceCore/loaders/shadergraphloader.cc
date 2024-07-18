//
// Created by Marcell on 07.07.2024.
//

#include <ceCore/loaders/shadergraphloader.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/graphics/material/material.hh>
#include <ceCore/graphics/shadergraph/shadergraph.hh>
#include <ceCore/graphics/shadergraph/ishadergraphcompiler.hh>
#include <ceCore/classregistry.hh>
#include <ceCore/resource/assetmanager.hh>

namespace ce
{

ShaderGraphLoader::ShaderGraphLoader()
    : BaseCEFAssetLoader()
{
  AddValidFile<Material>("SG");
  AddValidFile<Material>("SHADERGRAPH");
}


iObject *ShaderGraphLoader::Load(const CrimsonFile *file, const Class *cls, const ResourceLocator &locator) const
{
  const CrimsonFileElement *root = file->Root();
  const CrimsonFileElement *shaderGraphElement = root->GetChild("shaderGraph");
  auto sg = new ShaderGraph();
  AutoRelease autoRelSG(sg);


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


  const CrimsonFileElement *nodesElements = shaderGraphElement->GetChild("nodes");
  if (!nodesElements)
  {
    fprintf(stderr, "Invalid shader graph. No 'nodes' element\n");
    return nullptr;
  }

  for (size_t i = 0, in = nodesElements->GetNumberOfChildren(); i < in; ++i)
  {
    const CrimsonFileElement *childElement = nodesElements->GetChild(i);
    if (!childElement)
    {
      continue;
    }

    SGNode *node = nullptr;
    const std::string &tagName = childElement->GetTagName();
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

    if (!LoadNodeBindingsAndValues(childElement, node, sg))
    {
      return nullptr;
    }

  }

  const CrimsonFileElement *attributesElements = shaderGraphElement->GetChild("attributes");
  if (attributesElements)
  {
    LoadAttributes(attributesElements, sg, locator);
  }


  //
  // Now compile the shader graph to a material

  auto compilerFactory = ce::ObjectRegistry::Get<ce::iShaderGraphCompilerFactory>();
  if (compilerFactory)
  {
    auto compiler = compilerFactory->Create();
    if (compiler)
    {
      iShaderGraphCompiler::Parameters parameters{};
      memset(&parameters, 0, sizeof(parameters));
      parameters.DebugSources = true;
      return compiler->Compile(sg, parameters);
    }
  }

  return nullptr;
}

SGNode *ShaderGraphLoader::CreateNode(const ce::CrimsonFileElement *nodeElement, ce::ShaderGraph *sg) const
{
  const std::string &nodeTypeName = nodeElement->GetAttribute(0, "");
  if (nodeTypeName.empty())
  {
    return nullptr;
  }
  const Class *nodeTypeClass = ClassRegistry::Get()->GetClass(nodeTypeName);
  if (!nodeTypeClass)
  {
    fprintf(stderr, "Unable to loader shader graph: Class name '%s' node found.\n", nodeTypeName.c_str());
    return nullptr;
  }

  const std::string &nodeKey = nodeElement->GetAttribute(1, "");
  if (nodeKey.empty())
  {
    fprintf(stderr, "Unable to create node '%s'. No key defined.\n", nodeTypeName.c_str());
    return nullptr;
  }

  return sg->Add(nodeTypeClass, nodeKey);
}


SGResourceNode *
ShaderGraphLoader::CreateResourceNode(const ce::CrimsonFileElement *nodeElement, ce::ShaderGraph *sg) const
{
  const std::string &nodeTypeName = nodeElement->GetAttribute(0, "");
  if (nodeTypeName.empty())
  {
    return nullptr;
  }
  const Class *nodeTypeClass = ClassRegistry::Get()->GetClass(nodeTypeName);
  if (!nodeTypeClass)
  {
    fprintf(stderr, "Unable to loader shader graph: Class name '%s' node found.\n", nodeTypeName.c_str());
    return nullptr;
  }

  const std::string &nodeKey = nodeElement->GetAttribute(1, "");
  if (nodeKey.empty())
  {
    fprintf(stderr, "Unable to create node '%s'. No key defined.\n", nodeTypeName.c_str());
    return nullptr;
  }

  const std::string &nodeResourceName = nodeElement->GetAttribute(2, "");
  if (nodeResourceName.empty())
  {
    fprintf(stderr,
            "Unable to create node '%s'#'%s'. No resource name defined.\n",
            nodeTypeName.c_str(),
            nodeKey.c_str());
    return nullptr;
  }

  return sg->AddResource(nodeTypeClass, nodeKey, nodeResourceName);
}

bool ShaderGraphLoader::LoadNodeBindingsAndValues(const CrimsonFileElement *nodeElement,
                                                  SGNode *node,
                                                  ShaderGraph *sg) const
{
  for (size_t i = 0, in = nodeElement->GetNumberOfChildren(); i < in; i++)
  {
    const CrimsonFileElement *childElement = nodeElement->GetChild(i);
    if (!childElement)
    {
      continue;
    }

    const std::string &tagName = childElement->GetTagName();
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

bool ShaderGraphLoader::LoadValue(const CrimsonFileElement *valueElement,
                                  SGNode *node,
                                  ShaderGraph *sg) const
{
  size_t idx = valueElement->GetAttribute(0, 0xffff);
  if (idx == 0xffff)
  {
    fprintf(stderr, "Invalid value index of '%s'\n", node->GetKey().c_str());
    return false;
  }


  SGNodeInput *input = node->GetInput(idx);
  if (!input)
  {
    fprintf(stderr, "The index %zu is no valid input index of '%s'\n", idx, node->GetKey().c_str());
    return false;
  }

  float value = valueElement->GetAttribute(1, 0.0f);
  input->SetScalar(value);
  return true;
}


bool ShaderGraphLoader::LoadBinding(const CrimsonFileElement *valueElement,
                                    SGNode *node,
                                    ShaderGraph *sg) const
{
  size_t idx = valueElement->GetAttribute(0, 0xffff);
  if (idx == 0xffff)
  {
    fprintf(stderr, "Invalid value index of '%s'\n", node->GetKey().c_str());
    return false;
  }


  SGNodeInput *input = node->GetInput(idx);
  if (!input)
  {
    fprintf(stderr, "The index %zu is no valid input index of '%s'\n", idx, node->GetKey().c_str());
    return false;
  }

  std::string bindingInputKey = valueElement->GetAttribute(1, "");
  if (bindingInputKey.empty())
  {
    fprintf(stderr, "No binding input key is defined for '%s'\n", node->GetKey().c_str());
    return false;
  }

  SGNode *bindingInputNode = sg->GetNode(bindingInputKey);
  if (!bindingInputNode)
  {
    fprintf(stderr,
            "Binding input '%s' could not be found in shader graph for '%s'\n",
            bindingInputKey.c_str(),
            node->GetKey().c_str());
    return false;
  }

  size_t bindingInputOutputIdx = valueElement->GetAttribute(2, 0xffff);
  if (bindingInputOutputIdx == 0xffff)
  {
    fprintf(stderr, "Invalid value index of '%s'\n", node->GetKey().c_str());
    return false;
  }


  node->Bind(idx, bindingInputNode, bindingInputOutputIdx);
  return true;
}


void ShaderGraphLoader::LoadAttributes(const ce::CrimsonFileElement *attributesElement, ce::ShaderGraph *sg, const ResourceLocator &locator) const
{
  if (!attributesElement)
  {
    return;
  }
  for (size_t i = 0, in = attributesElement->GetNumberOfChildren(); i<in; ++i)
  {
    const CrimsonFileElement *attributeElement = attributesElement->GetChild(i);
    if (attributeElement->GetTagName() == std::string("attribute"))
    {
      LoadAttribute (attributeElement, sg, locator);
    }
  }
}

void ShaderGraphLoader::LoadAttribute(const ce::CrimsonFileElement *attributeElement, ce::ShaderGraph *sg, const ResourceLocator &locator) const
{
  if (attributeElement->GetNumberOfAttributes() < 2)
  {
    return;
  }
  std::string type = attributeElement->GetAttribute(0, "");
  std::string name = attributeElement->GetAttribute(1, "");
  if (type.empty() || name.empty())
  {
    return;
  }


  if (type == "Float")
  {
    float v = attributeElement->GetAttribute(2, 0.0f);
    sg->SetDefault(name, 1, &v);
  }
  else if (type == "Vec2")
  {
    Vector2f v {};
    v.x = attributeElement->GetAttribute(2, 0.0f);
    v.y = attributeElement->GetAttribute(3, 0.0f);
    sg->SetDefault(name, 2, &v.x);
  }
  else if (type == "Vec3")
  {
    Vector3f v {};
    v.x = attributeElement->GetAttribute(2, 0.0f);
    v.y = attributeElement->GetAttribute(3, 0.0f);
    v.z = attributeElement->GetAttribute(4, 0.0f);
    sg->SetDefault(name, 3, &v.x);
  }
  else if (type == "Vec4" || type == "Color4")
  {
    Vector4f v {};
    v.x = attributeElement->GetAttribute(2, 0.0f);
    v.y = attributeElement->GetAttribute(3, 0.0f);
    v.z = attributeElement->GetAttribute(4, 0.0f);
    v.w = attributeElement->GetAttribute(5, 0.0f);
    sg->SetDefault(name, 4, &v.x);
  }
  else if (type == "Texture")
  {
    std::string locatorStr = attributeElement->GetAttribute(2, "");
    if (!locatorStr.empty())
    {
      ResourceLocator txtLocator (locator, locatorStr);
      auto texture = AssetManager::Get()->Get<iTexture>(txtLocator);
      sg->SetDefault(name, texture);
    }
  }
}

} // ce