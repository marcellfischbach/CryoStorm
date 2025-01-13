
#include <csAssimpLoader/csAssimpSkeletonMeshLoader.hh>
#include <csAssimpLoader/csAssimpMaterialLoader.hh>
#include <csAssimpLoader/csAssimpConverter.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/resource/iFile.hh>
#include <csCore/graphics/csSkeletonMesh.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/csTypes.hh>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <map>
#include <string>
#include <vector>

namespace cs::assimp
{


struct SkeletonLoaderData
{
  const aiScene *scene = nullptr;
  std::map<std::string, Size> materialSlots;
  std::map<std::string, iMaterial*> defaultMaterials;
  csSkeletonMesh                    *mesh = nullptr;

};

csAssimpSkeletonMeshLoader::csAssimpSkeletonMeshLoader()
{

}


bool csAssimpSkeletonMeshLoader::CanLoad(const csClass *cls, const csAssetLocator &locator) const
{
  const std::string &ext = locator.GetExtension();
  return cls == csSkeletonMesh::GetStaticClass()
         && ext == std::string("FBX");
}


static void debug_node(aiNode *node, const csMatrix4f &parent, const std::string &indent)
{

//  Matrix4f local = ConvertMatrix4x4(node->mTransformation);
//  Matrix4f global = parent * local;
//
//
//  std::string line = indent + std::string (node->mName.C_Str());
//  std::string line_indent = std::string (line.length(), ' ');
//  printf ("%s %6.2f %6.2f %6.2f %6.2f\n",line.c_str(), local.m00, local.m01, local.m02, local.m03);
//  printf ("%s %6.2f %6.2f %6.2f %6.2f\n",line_indent.c_str(), local.m10, local.m11, local.m12, local.m13);
//  printf ("%s %6.2f %6.2f %6.2f %6.2f\n",line_indent.c_str(), local.m20, local.m21, local.m22, local.m23);
//  printf ("%s %6.2f %6.2f %6.2f %6.2f\n",line_indent.c_str(), local.m30, local.m31, local.m32, local.m33);
//  printf ("%s------------------------------------------\n", indent.c_str());
//  fflush(stdout);
//  for (int i = 0; i < node->mNumChildren; ++i)
//  {
//    debug_node (node->mChildren[i], global, indent + "     ");
//  }
}

iObject *csAssimpSkeletonMeshLoader::Load(const csClass *cls, const csAssetLocator &locator) const
{
  iFile *file = cs::csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!file)
  {
    return nullptr;
  }

  file->Seek(eSM_End, 0);
  long size = file->Tell();
  file->Seek(eSM_Set, 0);

  uint8_t *buffer         = new uint8_t[size];
  file->Read(sizeof(uint8_t), size, buffer);

  Assimp::Importer importer;
  const aiScene    *scene = importer.ReadFileFromMemory(buffer, size,
                                                        aiProcess_Triangulate
                                                        | aiProcess_CalcTangentSpace
                                                        | aiProcess_GenNormals
                                                        | aiProcess_MakeLeftHanded
                                                        | aiProcess_FlipWindingOrder
                                                        | aiProcess_JoinIdenticalVertices
                                                        | aiProcess_OptimizeMeshes
  );


  SkeletonLoaderData d;
  d.scene = scene;
  d.mesh  = new csSkeletonMesh();
  for (unsigned i = 0, in = scene->mNumMeshes; i < in; ++i)
  {
    aiMesh     *mesh     = scene->mMeshes[i];
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    aiString aiMatName;
    material->Get(AI_MATKEY_NAME, aiMatName);
    std::string materialName(aiMatName.C_Str());
    if (d.materialSlots.find(materialName) == d.materialSlots.end())
    {
      csAssetRef<iMaterial> mat;
      Size                  idx = d.mesh->AddMaterialSlot(materialName, mat);
      d.materialSlots[materialName] = idx;
      d.defaultMaterials[materialName] = csAssimpMaterialLoader::Read(material);
    }
  }


  csMatrix4f parentMatrix;
  parentMatrix.SetRotationY((float)M_PI);

  debug_node(scene->mRootNode, parentMatrix,  "");
  ReadSkeleton(scene->mRootNode, parentMatrix, d);
  ReadMesh(scene->mRootNode, parentMatrix, d);

  for (auto it = d.materialSlots.begin(); it!=d.materialSlots.end(); it++)
  {
    size_t                slotIdx  = it->second;
    csAssetRef<iMaterial> material = d.defaultMaterials[it->first];
    d.mesh->SetDefaultMaterial(slotIdx, material);
  }

  return d.mesh;
}

void csAssimpSkeletonMeshLoader::ReadSkeleton(aiNode *node,
                                              const csMatrix4f &parentMatrix,
                                              SkeletonLoaderData &d) const
{
  csMatrix4f localMatrix  = ConvertMatrix4x4(node->mTransformation);
  csMatrix4f globalMatrix = parentMatrix * localMatrix;

  std::string nodeName = std::string (node->mName.C_Str());
  if (nodeName == "Armature" || nodeName == "Skeleton")
  {
    for (unsigned i = 0, in = node->mNumChildren; i < in; ++i)
    {
      ReadBone(node->mChildren[i], d, csSkeleton::ILLEGAL_BONE_ID);
    }
    d.mesh->GetSkeleton().SetBase(globalMatrix);
    d.mesh->GetSkeleton().UpdateBones();
  }
  else
  {
    for (unsigned i = 0, in = node->mNumChildren; i < in; ++i)
    {
      ReadSkeleton(node->mChildren[i], globalMatrix, d);
    }
  }
}

void csAssimpSkeletonMeshLoader::ReadBone(aiNode *node,
                                          SkeletonLoaderData &d,
                                          size_t  parentBoneID) const
{

  csMatrix4f  localMatrix = ConvertMatrix4x4(node->mTransformation);
  std::string nodeName    = std::string (node->mName.C_Str());

  size_t boneID;

  csSkeleton &skeleton = d.mesh->GetSkeleton();
  if (parentBoneID == csSkeleton::ILLEGAL_BONE_ID)
  {
    boneID = skeleton.AddRoot(nodeName);
  }
  else
  {
    boneID = skeleton.AddChild(nodeName, parentBoneID);
  }

  csQuaternion     rotation = csQuaternion::FromMatrix(localMatrix);
  csSkeleton::Bone &bone    = skeleton.GetBone(boneID);
  bone.rotation = rotation;
  bone.poseRotation = rotation;
  bone.offset = localMatrix.GetTranslation();

  for (unsigned i = 0, in = node->mNumChildren; i < in; ++i)
  {
    ReadBone(node->mChildren[i], d, boneID);
  }

}

void csAssimpSkeletonMeshLoader::ReadMesh(aiNode *node, const csMatrix4f &parentMatrix, SkeletonLoaderData &d) const
{
  csMatrix4f localMatrix  = ConvertMatrix4x4(node->mTransformation);
  csMatrix4f globalMatrix = parentMatrix * localMatrix;


  for (unsigned i = 0, in = node->mNumMeshes; i < in; ++i)
  {
    aiMesh      *mesh        = d.scene->mMeshes[node->mMeshes[i]];
    iRenderMesh *renderMesh  = ConvertRenderMesh(mesh, globalMatrix, &d.mesh->GetSkeleton());
    aiMaterial  *material    = d.scene->mMaterials[mesh->mMaterialIndex];

    aiString aiMatName;
    material->Get(AI_MATKEY_NAME, aiMatName);
    std::string materialName(aiMatName.C_Str());
    Size        materialSlot = d.materialSlots[materialName];


    d.mesh->AddSubMesh(renderMesh, materialSlot);
  }


  for (unsigned i = 0, in = node->mNumChildren; i < in; ++i)
  {
    ReadMesh(node->mChildren[i], globalMatrix, d);
  }
}

}
