
#include <ceAssimpLoader/assimpmeshloader.hh>
#include <ceAssimpLoader/assimpmaterialloader.hh>
#include <ceAssimpLoader/assimpconverter.hh>
#include <ceCore/graphics/iDevice.hh>
#include <ceCore/graphics/csMesh.hh>
#include <ceCore/graphics/iRenderMesh.hh>
#include <ceCore/resource/ifile.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/types.hh>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <map>
#include <string>
#include <vector>


namespace cryo::assimp
{

struct StaticLoaderData
{
  const aiScene *scene = nullptr;
  std::map<std::string, Size> materialSlots;
  std::map<std::string, iMaterial*> defaultMaterials;
  csMesh                            *mesh = nullptr;

};

AssimpMeshLoader::AssimpMeshLoader()
{
  CS_CLASS_GEN_CONSTR;
}


bool AssimpMeshLoader::CanLoad(const Class *cls, const ResourceLocator &locator) const
{
  const std::string &ext = locator.GetExtension();
  return cls == csMesh::GetStaticClass()
         && ext == std::string("FBX");
}

static void debug_node (aiNode *node, const Matrix4f &parent, const std::string &indent)
{
//
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

iObject *AssimpMeshLoader::Load(const Class *cls, const ResourceLocator &locator) const
{
  iFile *file = cryo::VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
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


  StaticLoaderData d;
  d.scene = scene;
  d.mesh  = new csMesh();
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
      d.defaultMaterials[materialName] = AssimpMaterialLoader::Read(material);
    }
  }


  Matrix4f parentMatrix;
  parentMatrix.SetRotationY((float)M_PI);
  debug_node(scene->mRootNode, parentMatrix, "");
  ReadNode(scene->mRootNode, parentMatrix, d);

  for (auto it = d.materialSlots.begin(); it!=d.materialSlots.end(); it++)
  {
    size_t slotIdx = it->second;
    iMaterial* material = d.defaultMaterials[it->first];
    d.mesh->SetDefaultMaterial(slotIdx, material);
  }

  return d.mesh;
}

void AssimpMeshLoader::ReadNode(aiNode *node, const Matrix4f &parentMatrix, StaticLoaderData &d) const
{
  Matrix4f localMatrix  = ConvertMatrix4x4(node->mTransformation);
  Matrix4f globalMatrix = parentMatrix * localMatrix;
  
  


  for (unsigned i = 0, in = node->mNumMeshes; i < in; ++i)
  {
    aiMesh      *mesh        = d.scene->mMeshes[node->mMeshes[i]];
    iRenderMesh *renderMesh  = ConvertRenderMesh(mesh, globalMatrix, nullptr);
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
