//
// Created by Marcell on 05.04.2025.
//

#include <csAssimpImporter/AssimpSkeletonExporter.hh>
#include <assimp/scene.h>
#include <iostream>
#include <fstream>

namespace cs::imp
{


AssimpSkeletonExporter::AssimpSkeletonExporter(const aiScene *scene)
    : m_scene(scene)
{
  m_skeletonRootNames.insert("Armature"); // Blender Skeleton name
  m_skeletonRootNames.insert("Skeleton");
}

AssimpSkeletonExporter::~AssimpSkeletonExporter()
{
  for (auto bone: m_bones)
  {
    delete bone;
  }
  m_bones.clear();
}

void AssimpSkeletonExporter::SetSkeletonName(const std::set<std::string> &names)
{
  m_skeletonRootNames = names;
}

void AssimpSkeletonExporter::ScanBones()
{
  aiNode *root = FindRootNode(m_scene->mRootNode);
  if (!root)
  {
    return;
  }

  for (uint32_t i = 0, in = root->mNumChildren; i < in; i++)
  {
    ScanBones(root->mChildren[i], nullptr);
  }
}


void AssimpSkeletonExporter::ScanBones(aiNode *node, Bone *parent)
{
  std::string nodeName(node->mName.C_Str());
  uint32_t    boneId = m_bones.size();


  Bone *bone = new Bone();
  bone->id     = boneId;
  bone->name   = nodeName;
  bone->parent = parent;
  bone->node   = node;

  if (parent)
  {
    parent->children.push_back(bone);
  }

  m_bones.push_back(bone);

  for (uint32_t i = 0, in = node->mNumChildren; i < in; i++)
  {
    ScanBones(node->mChildren[i], bone);
  }
}

aiNode *AssimpSkeletonExporter::FindRootNode(aiNode *node)
{
  std::string nodeName(node->mName.C_Str());
  if (m_skeletonRootNames.contains(nodeName))
  {
    return node;
  }

  for (uint32_t i = 0, in = node->mNumChildren; i < in; i++)
  {
    aiNode *rootNode = FindRootNode(node->mChildren[i]);
    if (rootNode)
    {
      return rootNode;
    }
  }
  return nullptr;
}

bool AssimpSkeletonExporter::HasBones() const
{
  return !m_bones.empty();
}

uint32_t AssimpSkeletonExporter::GetBoneIndex(const std::string &name) const
{
  for (const auto &bone: m_bones)
  {
    if (bone->name == name)
    {
      return bone->id;
    }
  }
  return IllegalBoneID;
}

void AssimpSkeletonExporter::Export(const std::string &filename) const
{
  std::ofstream out;
  out.open(filename, std::ios::out | std::ios::trunc);


  out << "skeleton {" << std::endl;

  for (auto bone: m_bones)
  {
    if (!bone->parent)
    {
      ExportBone(out, bone, "  ");
    }
  }

  out << "}" << std::endl;

  out.close();
}

void AssimpSkeletonExporter::ExportBone(std::ofstream &out, Bone *bone, std::string indent) const
{
  aiMatrix4x4 &tr = bone->node->mTransformation;
  out << indent << "bone id: " << bone->id << " name:\"" << bone->name << "\" {" << std::endl
      << indent << "  transform {" << std::endl
      << indent << "    matrix4 " << tr.a1 << " " << tr.b1 << " " << tr.c1 << "  " << tr.d1 << std::endl
      << indent << "            " << tr.a2 << " " << tr.b2 << " " << tr.c2 << "  " << tr.d2 << std::endl
      << indent << "            " << tr.a3 << " " << tr.b3 << " " << tr.c3 << "  " << tr.d3 << std::endl
      << indent << "            " << tr.a4 << " " << tr.b4 << " " << tr.c4 << "  " << tr.d4 << ", " << std::endl
      << indent << "  }," << std::endl;

  if (!bone->children.empty())
  {
    out << indent << "  children {" << std::endl;
    for (auto childBone: bone->children)
    {
      ExportBone(out, childBone, indent + "    ");
    }
    out << indent << "  }," << std::endl;
  }

  out << indent << "}," << std::endl;
}

}