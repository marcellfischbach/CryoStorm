#include <ceAssimpLoader/assimpconverter.hh>
#include <ceCore/math/vector4i.hh>
#include <ceCore/graphics/irendermesh.hh>
#include <ceCore/graphics/skeleton.hh>
#include <ceCore/objectregistry.hh>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include <vector>

namespace ce::assimp
{


Color4f ConvertRGBA(aiColor4D &v)
{
  return {v.r, v.g, v.b, v.a};
}

Vector3f Convert3f(aiVector3D &v)
{
  return {v.x, v.y, v.z};
}

Vector2f Convert2f(aiVector3D &v)
{
  return {v.x, v.y};
}


Matrix4f ConvertMatrix4x4(aiMatrix4x4 &aiMat)
{
#if 0
  return Matrix4f(
      aiMat.a1, aiMat.a2, aiMat.a3, aiMat.a4,
      aiMat.b1, aiMat.b2, aiMat.b3, aiMat.b4,
      aiMat.c1, aiMat.c2, aiMat.c3, aiMat.c4,
      aiMat.d1, aiMat.d2, aiMat.d3, aiMat.d4
  );
#else
  return Matrix4f(
      aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
      aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
      aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
      aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
  );
#endif
}


struct Weight
{
  Vector4i bones;
  Vector4f weight;
};


Weight get_weight(aiMesh *mesh, unsigned vertexID, Skeleton *skeleton)
{
  struct TempWeight
  {
    int   boneID;
    float weight;
    bool operator<(const TempWeight &other) const
    {
      return weight > other.weight;
    }
  };

  struct
  {
    bool operator()(TempWeight &a, TempWeight &b) const { return a.weight > b.weight; }
  } customLess;

  std::vector<TempWeight> weights;
  for (int                i = 0; i < mesh->mNumBones; ++i)
  {
    aiBone   *bone = mesh->mBones[i];
    for (int w     = 0; w < bone->mNumWeights; ++w)
    {
      aiVertexWeight &weight = bone->mWeights[w];
      if (weight.mVertexId == vertexID)
      {
        size_t boneIndex = i; //skeleton.IndexOf(std::string(bone->mName.C_Str()));
        if (boneIndex != Skeleton::ILLEGAL_BONE_ID)
        {
          TempWeight tWeight {
              (int) boneIndex,
              weight.mWeight
          };
          weights.push_back(tWeight);
        }
      }
    }
  }


  std::sort(weights.begin(), weights.end(), customLess);
  Weight res;
  if (!weights.empty())
  {
    res.bones.x  = weights[0].boneID;
    res.weight.x = weights[0].weight;
  }
  else
  {
    res.bones.x = 0;
    res.weight.x = 0;
  }
  if (weights.size() > 1)
  {
    res.bones.y  = weights[1].boneID;
    res.weight.y = weights[1].weight;
  }
  else
  {
    res.bones.y = 0;
    res.weight.y = 0;
  }
  if (weights.size() > 2)
  {
    res.bones.z  = weights[2].boneID;
    res.weight.z = weights[2].weight;
  }
  else
  {
    res.bones.z= 0;
    res.weight.z = 0;
  }
  if (weights.size() > 3)
  {
    res.bones.w  = weights[3].boneID;
    res.weight.w = weights[3].weight;
  }
  else
  {
    res.bones.w = 0;
    res.weight.w = 0;
  }

  float weight_sum = res.weight.x + res.weight.y + res.weight.z + res.weight.w;
  if (weight_sum != 0.0f)
  {
    res.weight /= weight_sum;
  }

  return res;
}

iRenderMesh *ConvertRenderMesh(aiMesh *mesh, const Matrix4f &matrix2, Skeleton* skeleton)
{
  Matrix4f matrix;
  std::vector<Vector3f> vertices;
  std::vector<Vector3f> normals;
  std::vector<Vector3f> tangents;
  std::vector<Vector2f> uvs;
  std::vector<Vector4f> weights;
  std::vector<Vector4i> bones;
  std::vector<Color4f>  colors;

  for (unsigned i = 0, in = mesh->mNumVertices; i < in; ++i)
  {
    Vector3f vertex = Convert3f(mesh->mVertices[i]);
    vertices.push_back(Matrix4f::Transform(matrix, vertex));
//    printf("[%d] %.2f %.2f %.2f", i, vertex.x, vertex.y, vertex.z);

    if (mesh->mNormals)
    {
      Vector3f normal = Convert3f(mesh->mNormals[i]);
      normal.Normalize();
      normals.push_back(Matrix4f::Mult(matrix, normal));
//      printf("   %.2f %.2f %.2f", normal.x, normal.y, normal.z);
    }
    if (mesh->mTangents)
    {
      Vector3f tangent = Convert3f(mesh->mTangents[i]);
      tangent.Normalize();
      tangents.push_back(Matrix4f::Mult(matrix, tangent));
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
      colors.emplace_back(1, 1, 1, 1);
    }
    
    if (skeleton)
    {
      const Weight &weight = get_weight(mesh, i, skeleton);
      bones.push_back(weight.bones);
      weights.push_back(weight.weight);
    }

//    printf("\n");
  }

  std::vector<uint32_t> indices;
  for (unsigned         i = 0, in = mesh->mNumFaces; i < in; ++i)
  {
    aiFace face = mesh->mFaces[i];
    if (face.mNumIndices == 3)
    {
      indices.push_back(face.mIndices[0]);
      indices.push_back(face.mIndices[1]);
      indices.push_back(face.mIndices[2]);
    }

  }


  auto                 renderMeshGenFact = ObjectRegistry::Get<iRenderMeshGeneratorFactory>();
  iRenderMeshGenerator *generator        = renderMeshGenFact->Create();
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
  iRenderMesh *renderMesh = generator->Generate();
  generator->Release();

  return renderMesh;
}


}