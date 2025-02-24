//
// Created by Marcell on 02.01.2025.
//
#include <cmath>

#include <csEditor/editors/shadergraph/ShaderGraphSaver.hh>
#include <csCore/graphics/shadergraph/csShaderGraph.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <csCore/resource/csVFS.hh>
#include <string>
#include <sstream>

using namespace cs;

ShaderGraphSaver::ShaderGraphSaver(cs::csShaderGraph *shaderGraph)
    : m_shaderGraph(shaderGraph)
{
  CollectNodes();
}

void ShaderGraphSaver::CollectNodes()
{
  m_sortedNodes.clear();
  std::vector<csSGNode *> nodes;
  for (int                i = 0; i < m_shaderGraph->GetNumberOfNodes(); ++i)
  {
    nodes.push_back(m_shaderGraph->GetNode(i));
  }


  while (!nodes.empty())
  {
    size_t    size = nodes.size();
    for (auto node: nodes)
    {
      bool     add = true;
      for (int i   = 0; i < node->GetNumberOfInputs(); ++i)
      {
        csSGNodeInput *input = node->GetInput(i);
        if (input)
        {
          csSGNodeOutput *output = input->GetSource();
          if (output)
          {
            csSGNode *outputNode = output->GetNode();
            if (std::find(m_sortedNodes.begin(), m_sortedNodes.end(), outputNode) == m_sortedNodes.end())
            {
              add = false;
              break;
            }
          }
        }
      }
      if (add)
      {
        m_sortedNodes.push_back(node);
        nodes.erase(std::find(nodes.begin(), nodes.end(), node));
        break;
      }
    }
    if (size == nodes.size())
    {
      break;
    }
  }

}


std::string to_string(eRenderQueue queue)
{
  switch (queue)
  {
    case cs::eRenderQueue::Default:
      return "Default";
    case cs::eRenderQueue::Transparency:
      return "Transparency";
  }
  return "Unknown";
}

std::string to_string(csShaderGraph::eLightingMode lightingMode)
{
  switch (lightingMode)
  {
    case csShaderGraph::eLightingMode::eLM_Default:
      return "Default";
    case csShaderGraph::eLightingMode::eLM_Attenuated:
      return "Attenuated";
    case csShaderGraph::eLightingMode::eLM_Unlit:
      return "Unlit";
  }
  return "Unknown";
}


std::string to_string(csShaderGraph::eBlendingMode blendingMode)
{
  switch (blendingMode)
  {
    case csShaderGraph::eBlendingMode::eBM_Add:
      return "Add";
    case csShaderGraph::eBlendingMode::eBM_Alpha:
      return "Alpha";
    case csShaderGraph::eBlendingMode::eBM_Off:
      return "Off";
  }
  return "Unknown";
}

std::string to_string(eMaterialAttributeType matType)
{
  switch (matType)
  {
    case eMAT_Float:
      return "Float";
    case eMAT_Vec2:
      return "Vec2";
    case eMAT_Vec3:
      return "Vec3";
    case eMAT_Vec4:
      return "Vec4";
    case eMAT_Int:
      return "Int";
    case eMAT_IVec2:
      return "IVec2";
    case eMAT_IVec3:
      return "IVec3";
    case eMAT_IVec4:
      return "IVec4";
    case eMAT_Matrix3:
      return "Mat3";
    case eMAT_Matrix4:
      return "Mat4";
    case eMAT_Texture:
      return "Texture";
    case eMAT_Undefined:
    default:
      return "Undefined";

  }
  return "Unknown";
}

std::string ShaderGraphSaver::GetNodeSource(cs::csSGNode *node)
{
  std::stringstream stream;
  if (node->IsInstanceOf<csSGResourceNode>())
  {
    csSGResourceNode *resourceNode = csQueryClass<csSGResourceNode>(node);
    stream
        << "    resource"
        << " \"" << node->GetClass()->GetName() << "\""
        << " " << node->GetKey()
        << " \"" << resourceNode->GetResourceName() << "\"";
  }
  else if (m_shaderGraph->Root() == node)
  {
    stream
        << "    shaderGraph";
  }
  else
  {
    stream
        << "    node"
        << " \"" << node->GetClass()->GetName() << "\""
        << " " << node->GetKey();
  }
  stream << " x:" << std::to_string(node->GetPosition().x)
         << " y:" << std::to_string(node->GetPosition().y)
         << " {\n";

  if (node->IsInstanceOf<csSGResourceNode>())
  {
    csSGResourceNode            *resourceNode = csQueryClass<csSGResourceNode>(node);
    const std::array<int, 4>    &is           = resourceNode->GetDefaultInts();
    const std::array<float, 16> &fs           = resourceNode->GetDefaultFloats();

    switch (resourceNode->GetMatType())
    {
      case cs::eMAT_Int:
        stream << "      defaultInt " << is[0] << ",\n";
        break;
      case cs::eMAT_IVec2:
        stream << "      defaultInt " << is[0] << " " << is[1] << ",\n";
        break;
      case cs::eMAT_IVec3:
        stream << "      defaultInt" << is[0] << " " << is[1] << " " << is[2] << ",\n";
        break;
      case cs::eMAT_IVec4:
        stream << "      defaultInt " << is[0] << " " << is[1] << " " << is[2] << " " << is[3] << ",\n";
        break;
      case cs::eMAT_Float:
        stream << "      defaultFloat " << fs[0] << ",\n";
        break;
      case cs::eMAT_Vec2:
        stream << "      defaultFloat " << fs[0] << " " << fs[1] << ",\n";
        break;
      case cs::eMAT_Vec3:
        stream << "      defaultFloat " << fs[0] << " " << fs[1] << " " << fs[2] << ",\n";
        break;
      case cs::eMAT_Vec4:
        stream << "      defaultFloat " << fs[0] << " " << fs[1] << " " << fs[2] << " " << fs[3] << ",\n";
        break;
      case cs::eMAT_Matrix3:
        stream << "      defaultFloat " << fs[0] << " " << fs[1] << " " << fs[2] << "\n"
               << "                   " << fs[3] << " " << fs[4] << " " << fs[5] << "\n"
               << "                   " << fs[6] << " " << fs[7] << " " << fs[8] << ",\n";
        break;
      case cs::eMAT_Matrix4:
        stream << "      defaultFloat " << fs[0] << " " << fs[1] << " " << fs[2] << "  " << fs[3] << "\n"
               << "                   " << fs[4] << " " << fs[5] << " " << fs[6] << "  " << fs[7] << "\n"
               << "                   " << fs[8] << " " << fs[9] << " " << fs[10] << "  " << fs[11] << "\n"
               << "                   " << fs[12] << " " << fs[13] << " " << fs[14] << "  " << fs[15] << ",\n";
        break;
      case cs::eMAT_Texture:
        stream << "      defaultLocator \"" << resourceNode->GetDefaultLocator().Encoded() << "\",\n";
        break;
      case cs::eMAT_Undefined:
        break;

    }

  }
  for (int i = 0; i < node->GetNumberOfInputs(); ++i)
  {
    csSGNodeInput *input = node->GetInput(i);
    if (!input)
    {
      continue;
    }

    csSGNodeOutput *output = input->GetSource();
    if (output)
    {
      csSGNode *outputNode = output->GetNode();
      stream
          << "      binding "
          << " \"" << input->GetName() << "\""
          << " " << outputNode->GetKey()
          << " \"" << output->GetName() << "\""
          << ",\n";
    }
    else
    {
      stream
          << "      value \"" << input->GetName() << "\""
          << " " << std::to_string(input->GetScalar())
          << ",\n";
    }
  }

  stream << "    },\n";

  return stream.str();
}

std::string ShaderGraphSaver::GetAttributeSource(cs::csSGNode *node)
{
  std::stringstream stream;
  if (node->IsInstanceOf<csSGResourceNode>())
  {
    csSGResourceNode *resourceNode = csQueryClass<csSGResourceNode>(node);
    stream
        << "    attribute"
        << " " << to_string(resourceNode->GetMatType())
        << " \"" << resourceNode->GetResourceName() << "\""
        << ",\n";
  }

  return stream.str();
}

std::string ShaderGraphSaver::GetShaderGraphSource()
{
  std::stringstream stream;
  stream << "shaderGraph {\n\n"
         << "  receiveShadow " << (m_shaderGraph->IsReceiveShadow() ? "1" : "0") << ",\n"
         << "  queue " << to_string(m_shaderGraph->GetRenderQueue()) << ",\n"
         << "  lighting " << to_string(m_shaderGraph->GetLightingMode()) << ",\n"
         << "  blending " << to_string(m_shaderGraph->GetBlendingMode()) << ",\n\n";

  stream
      << "  nodes {\n";

  for (auto node: m_sortedNodes)
  {
    stream << GetNodeSource(node);
  }
  stream << GetNodeSource(m_shaderGraph->Root());

  stream
      << "  }\n"
      << "}\n";
  return stream.str();
};


void ShaderGraphSaver::Save(const cs::csAssetLocator &locator)
{
  std::string          source = GetShaderGraphSource();
  const csOwned<iFile> &owned = csVFS::Get()->Open(locator, eAM_Write, eOM_Binary);
  owned->Write(sizeof(char), source.length(), source.c_str());


//  iFile *file = csVFS::Get()->Open(m_locator, eAM_Write, eAF_Text);
//  if (!file)
//  {
//    return;
//  }
//
//  const std::string &string = generate_shader_graph_code(m_shaderGraph);
//  file->Write(string.c_str(), string.size());
//  file->Close();

}