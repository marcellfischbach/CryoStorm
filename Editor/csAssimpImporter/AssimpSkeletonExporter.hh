//
// Created by Marcell on 05.04.2025.
//

#pragma once

#include <set>
#include <string>
#include <vector>
#include <assimp/matrix4x4.h>

struct aiScene;
struct aiNode;

namespace cs::imp
{

class AssimpSkeletonExporter
{
public:
  struct BoneDecl
  {
    uint32_t idx;
    std::string name;
  };
public:
  AssimpSkeletonExporter(const aiScene* scene);
  ~AssimpSkeletonExporter();
  void SetSkeletonName (const std::set<std::string> &names);

  void ScanBones ();
  bool  HasBones () const;


  uint32_t GetBoneIndex (const std::string &name) const;
  std::vector<BoneDecl> GetBoneDecl () const;

  static const uint32_t IllegalBoneID = ~0x00;

  void Export(const std::string &filename) const;



private:
  aiNode* FindRootNode (aiNode* node);

  void WriteTransform (std::ofstream &out, aiMatrix4x4 &matrix, std::string indent) const;


  struct Bone
  {
    uint32_t id;
    std::string name;
    Bone* parent;
    aiNode* node;
    std::vector<Bone*> children;
  };
  void ScanBones (aiNode* node, Bone* parent);
  void ExportBone (std::ofstream &out, Bone* bone, std::string indent) const;

  std::vector<Bone*> m_bones;
  aiNode* m_skeletonRoot;

  const aiScene* m_scene;
  std::set<std::string> m_skeletonRootNames;
};

}
