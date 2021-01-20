
#include <spcAssimpLoader/assimpmeshloader.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/mesh.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/resource/ifile.hh>
#include <spcCore/resource/vfs.hh>
#include <spcCore/objectregistry.hh>
#include <spcCore/types.hh>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <map>
#include <string>
#include <vector>

namespace spc::assimp
{

void AssimpMeshLoaderConvert(aiMatrix4x4& aiMat, Matrix4f& out);
iRenderMesh* AssimpMeshLoaderLoadMesh(aiMesh* mesh, const Matrix4f& matrix);

struct LoaderData
{
  const aiScene* scene;
  std::map<std::string, Size> materialSlots;
  Mesh* mesh;

};

AssimpMeshLoader::AssimpMeshLoader()
{
  SPC_CLASS_GEN_CONSTR;
}


bool AssimpMeshLoader::CanLoad(const Class* cls, const ResourceLocator& locator) const
{
  const std::string& ext = locator.GetExtension();
  return cls == Mesh::GetStaticClass()
    && ext == std::string("FBX");
}

iObject* AssimpMeshLoader::Load(const Class* cls, const ResourceLocator& locator) const
{
  iFile* file = spc::VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!file)
  {
    return nullptr;
  }

  file->Seek(eSM_End, 0);
  long size = file->Tell();
  file->Seek(eSM_Set, 0);

  UInt8* buffer = new UInt8[size];
  file->Read(sizeof(UInt8), size, buffer);

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFileFromMemory(buffer, size,
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate |
    aiProcess_GenNormals |
    aiProcess_MakeLeftHanded |
    aiProcess_FlipWindingOrder |
    aiProcess_JoinIdenticalVertices);


  
  

  LoaderData d;
  d.scene = scene;
  d.mesh = new Mesh();
  for (unsigned i = 0, in = scene->mNumMeshes; i < in; ++i)
  {
    aiMesh* mesh = scene->mMeshes[i];
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

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

void AssimpMeshLoader::ReadNode(aiNode* node, const Matrix4f &parentMatrix, LoaderData& d) const
{
  Matrix4f localMatrix;
  AssimpMeshLoaderConvert(node->mTransformation, localMatrix);
  Matrix4f globalMatrix = parentMatrix * localMatrix;


  for (unsigned i = 0, in = node->mNumMeshes; i < in; ++i)
  {
    aiMesh* mesh = d.scene->mMeshes[node->mMeshes[i]];
    iRenderMesh* renderMesh = AssimpMeshLoaderLoadMesh(mesh, globalMatrix);
    aiMaterial* material = d.scene->mMaterials[mesh->mMaterialIndex];

    aiString aiMatName;
    material->Get(AI_MATKEY_NAME, aiMatName);
    std::string materialName(aiMatName.C_Str());
    Size materialSlot = d.materialSlots[materialName];


    d.mesh->AddSubMesh(renderMesh, materialSlot);
  }


  


  for (unsigned i = 0, in = node->mNumChildren; i < in; ++i)
  {
    ReadNode(node->mChildren[i], globalMatrix, d);
  }
}



void AssimpMeshLoaderConvert(aiMatrix4x4& aiMat, Matrix4f& out) 
{
  out.Set(
    aiMat.a1, aiMat.a2, aiMat.a3, aiMat.a4,
    aiMat.b1, aiMat.b2, aiMat.b3, aiMat.b4,
    aiMat.c1, aiMat.c2, aiMat.c3, aiMat.c4,
    aiMat.d1, aiMat.d2, aiMat.d3, aiMat.d4
  );
}

Color4f ConvertRGBA(aiColor4D& v)
{
  return Color4f(v.r, v.g, v.b, v.a);
}

Vector3f Convert3f(aiVector3D& v)
{
  return Vector3f(v.x, v.y, v.z);
}

Vector2f Convert2f(aiVector3D& v)
{
  return Vector2f(v.x, v.y);
}

iRenderMesh* AssimpMeshLoaderLoadMesh(aiMesh* mesh, const Matrix4f &matrix)
{

  std::vector<Vector3f> vertices;
  std::vector<Vector3f> normals;
  std::vector<Vector3f> tangents;
  std::vector<Vector2f> uvs;
  std::vector<Color4f>  colors;

  for (unsigned i = 0, in = mesh->mNumVertices; i < in; ++i)
  {
    Vector3f vertex = Convert3f(mesh->mVertices[i]);
    vertices.push_back(Matrix4f::Transform(matrix, vertex, vertex));
    printf("[%d] %.2f %.2f %.2f", i, vertex.x, vertex.y, vertex.z);

    if (mesh->mNormals)
    {
      Vector3f normal = Convert3f(mesh->mNormals[i]);
      normal.Normalize();
      normals.push_back(Matrix4f::Mult(matrix, normal, normal));
      printf("   %.2f %.2f %.2f", normal.x, normal.y, normal.z);
    }
    if (mesh->mTangents)
    {
      Vector3f tangent = Convert3f(mesh->mTangents[i]);
      tangent.Normalize();
      tangents.push_back(Matrix4f::Mult(matrix, tangent, tangent));
    }

    if (mesh->mTextureCoords[0])
    {
      Vector2f uv = Convert2f(mesh->mTextureCoords[0][i]);
      uvs.push_back(uv);
    }
    if (mesh->mColors[0])
    {
      Color4f color = ConvertRGBA(mesh->mColors[0][i]);
      colors.push_back(color);
    }
    else
    {
      colors.push_back(Color4f(1, 1, 1, 1));
    }
    printf("\n");
  }

  std::vector<UInt32> indices;
  for (unsigned i = 0, in = mesh->mNumFaces; i < in; ++i)
  {
    aiFace face = mesh->mFaces[i];
    if (face.mNumIndices == 3)
    {
      indices.push_back(face.mIndices[0]);
      indices.push_back(face.mIndices[1]);
      indices.push_back(face.mIndices[2]);
    }

  }


  auto renderMeshGenFact = ObjectRegistry::Get<iRenderMeshGeneratorFactory>();
  iRenderMeshGenerator* generator = renderMeshGenFact->Create();
  generator->SetVertices(vertices);
  if (mesh->mNormals)
  {
    generator->SetNormals(normals);
  }
  if (mesh->mTangents)
  {
    generator->SetTangents(tangents);
  }
  if (mesh->mTextureCoords[0])
  {
    generator->SetUV0(uvs);
  }
  if (!colors.empty())
  {
    generator->SetColors(colors);
  }
  
  generator->SetIndices(indices);
  iRenderMesh* renderMesh = generator->Generate();
  generator->Release();

  return renderMesh;
}

}
