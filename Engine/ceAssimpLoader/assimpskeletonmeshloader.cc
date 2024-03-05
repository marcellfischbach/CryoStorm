
#include <ceAssimpLoader/assimpskeletonmeshloader.hh>
#include <ceAssimpLoader/assimpconverter.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/irendermesh.hh>
#include <ceCore/resource/ifile.hh>
#include <ceCore/graphics/skeletonmesh.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/types.hh>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <map>
#include <string>
#include <vector>

namespace ce::assimp
{


struct LoaderData
{
  const aiScene *scene = nullptr;
  std::map<std::string, Size> materialSlots;
  SkeletonMesh *mesh = nullptr;

};

AssimpSkeletonMeshLoader::AssimpSkeletonMeshLoader()
{
  CE_CLASS_GEN_CONSTR;
}


bool AssimpSkeletonMeshLoader::CanLoad(const Class *cls, const ResourceLocator &locator) const
{
  const std::string &ext = locator.GetExtension();
  return cls == SkeletonMesh::GetStaticClass()
         && ext == std::string("FBX");
}

iObject *AssimpSkeletonMeshLoader::Load(const Class *cls, const ResourceLocator &locator) const
{
  iFile *file = ce::VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
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


  LoaderData d;
  d.scene = scene;
  d.mesh  = new SkeletonMesh();
  for (unsigned i = 0, in = scene->mNumMeshes; i < in; ++i)
  {
    aiMesh     *mesh     = scene->mMeshes[i];
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    aiString aiMatName;
    material->Get(AI_MATKEY_NAME, aiMatName);
    std::string materialName(aiMatName.C_Str());
    if (d.materialSlots.find(materialName) == d.materialSlots.end())
    {
      Size idx = d.mesh->AddMaterialSlot(materialName);
      d.materialSlots[materialName] = idx;
    }
  }





  Matrix4f parentMatrix;
  ReadNode(scene->mRootNode, parentMatrix, d);


  return d.mesh;
}

void AssimpSkeletonMeshLoader::ReadNode(aiNode *node, const Matrix4f &parentMatrix, LoaderData &d) const
{
  Matrix4f localMatrix  = ConvertMatrix4x4(node->mTransformation);
  Matrix4f globalMatrix = parentMatrix * localMatrix;


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
    ReadNode(node->mChildren[i], globalMatrix, d);
  }
}

}
