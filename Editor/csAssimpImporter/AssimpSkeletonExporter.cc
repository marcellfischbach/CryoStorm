//
// Created by Marcell on 05.04.2025.
//

#include <csAssimpImporter/AssimpSkeletonExporter.hh>
#include <assimp/scene.h>
#include <csCore/math/csMatrix4f.hh>
#include <csCore/math/csQuaternion.hh>
#include <iostream>
#include <fstream>
#include <iomanip>

namespace cs::imp
{


static csMatrix4f to_matrix4f(const aiMatrix4x4 &mat)
{
  return csMatrix4f(
      mat.a1, mat.b1, mat.c1, mat.d1,
      mat.a2, mat.b2, mat.c2, mat.d2,
      mat.a3, mat.b3, mat.c3, mat.d3,
      mat.a4, mat.b4, mat.c4, mat.d4
  );
}

static csQuaternion to_quaternion(const aiQuaternion &q)
{
  return csQuaternion(q.x, q.y, q.z, q.w);
}


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
  m_skeletonRoot = FindRootNode(m_scene->mRootNode);
  if (!m_skeletonRoot)
  {
    return;
  }
//  aiMatrix4x4  root;
//  ScanBones(m_skeletonRoot, nullptr, root, "");

  for (uint32_t i = 0, in = m_skeletonRoot->mNumChildren; i < in; i++)
  {
    aiMatrix4x4  root = m_skeletonRoot->mTransformation;
    ScanBones(m_skeletonRoot->mChildren[i], nullptr, root, "");
  }
}



void AssimpSkeletonExporter::ScanBones(aiNode *node, Bone *parent, const aiMatrix4x4 &parentMatrix, const std::string &indent)
{
  std::string nodeName(node->mName.C_Str());
  uint32_t    boneId = m_bones.size();


  Bone *bone = new Bone();
  bone->id     = boneId;
  bone->name   = nodeName;
  bone->parent = parent;
  bone->node   = node;
  bone->globalMatrix = parentMatrix * node->mTransformation;
  to_matrix4f(bone->globalMatrix).Debug(nodeName.c_str(), indent.c_str());

  if (parent)
  {
    parent->children.push_back(bone);
  }

  m_bones.push_back(bone);

  for (uint32_t i = 0, in = node->mNumChildren; i < in; i++)
  {
    ScanBones(node->mChildren[i], bone, bone->globalMatrix, indent + "  ");
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

aiMatrix4x4 AssimpSkeletonExporter::GetBoneMatrix(const std::string &boneName) const
{
  for (const auto &bone: m_bones)
  {
    if (bone->name == boneName)
    {
      return bone->globalMatrix;
    }
  }
  return aiMatrix4x4();
}

aiMatrix4x4  AssimpSkeletonExporter::GetRootMatrix() const
{
  return m_skeletonRoot->mTransformation;
}

std::vector<AssimpSkeletonExporter::BoneDecl> AssimpSkeletonExporter::GetBoneDecl() const
{
  std::vector<BoneDecl> decl;
  for (const auto       bone: m_bones)
  {
    decl.emplace_back(bone->id, bone->name);
  }

  return decl;
}


void AssimpSkeletonExporter::Export(const std::string &filename) const
{
  std::ofstream out;
  out.open(filename, std::ios::out | std::ios::trunc);


  out << "skeleton {" << std::endl;

  if (m_skeletonRoot)
  {
    WriteTransform(out, m_skeletonRoot->mTransformation, "  ");
//    aiMatrix4x4 mat;
//    WriteTransform(out, mat , "  ");
  }
  out << "  bones  {" << std::endl;

  for (auto bone: m_bones)
  {
    if (!bone->parent)
    {
      ExportBone(out, bone, "    ");
    }
  }

  out << "  }" << std::endl
      << "}" << std::endl;

  out.close();
}

void AssimpSkeletonExporter::WriteTransform(std::ofstream &out, aiMatrix4x4 &tr, std::string indent) const
{
  out << indent << "transform {" << std::endl
      << indent << "  matrix4 " << std::fixed << tr.a1 << " " << tr.b1 << " " << tr.c1 << "  " << tr.d1 << std::endl
      << indent << "          " << tr.a2 << " " << tr.b2 << " " << tr.c2 << "  " << tr.d2 << std::endl
      << indent << "          " << tr.a3 << " " << tr.b3 << " " << tr.c3 << "  " << tr.d3 << std::endl
      << indent << "          " << tr.a4 << " " << tr.b4 << " " << tr.c4 << "  " << tr.d4 << ", " << std::endl
      << indent << "}," << std::endl;
}

void AssimpSkeletonExporter::ExportBone(std::ofstream &out, Bone *bone, std::string indent) const
{
  aiMatrix4x4 &tr = bone->node->mTransformation;
  out << indent << "bone id: " << bone->id << " name:\"" << bone->name << "\" {" << std::endl;
  WriteTransform(out, tr, indent + "  ");

  csMatrix4f mat = to_matrix4f(tr);
  mat.Debug(bone->name.c_str());
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