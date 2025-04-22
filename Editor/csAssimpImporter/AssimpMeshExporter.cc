//
// Created by mcel on 04.03.2025.
//

#include <csAssimpImporter/AssimpMeshExporter.hh>
#include <csCryoFile/csCryoFile.hh>
#include <csCore/math/csVector.hh>
#include <csCore/math/csMatrix.hh>
#include <sstream>
#include <ostream>

using namespace cs::file;


namespace cs::imp
{

AssimpMeshExporter::AssimpMeshExporter(const aiScene *scene)
    : m_scene(scene)
    , m_skeletonExporter(scene)
{
  m_skeletonExporter.ScanBones();
  csMatrix4f id;
  ScanMatrices(m_scene->mRootNode, id);
}


void AssimpMeshExporter::combine(const aiNode *node)
{
  aiMatrix4x4 root;
  combine(node, root);
}


void AssimpMeshExporter::combine(const aiNode *node, const aiMatrix4x4 &parent)
{
  aiMatrix4x4 mat = parent * node->mTransformation;
  for (int    i   = 0; i < node->mNumMeshes; ++i)
  {
    unsigned int meshId = node->mMeshes[i];
    if (meshId >= m_scene->mNumMeshes)
    {
      continue;
    }
    aiMesh *mesh = m_scene->mMeshes[meshId];
    if (!mesh)
    {
      continue;
    }

    PushMesh(mat, mesh);

  }

  for (int i = 0; i < node->mNumChildren; ++i)
  {
    aiNode *child = node->mChildren[i];
    combine(child, mat);
  }
}

void AssimpMeshExporter::PushMesh(const aiMatrix4x4 &mat, const aiMesh *mesh)
{
  VertexDecl vertDecl = ExtractVertexDecl(mesh);
  MeshData   &data    = FindMeshData(vertDecl, TRIANGLES, mesh->mMaterialIndex);

  Push(data, mesh, mat);
}


void AssimpMeshExporter::PushTexCoord(std::vector<aiVector3D> &values,
                                      aiVector3D *srcValues,
                                      uint32_t srcValueCount)
{
  if (!srcValues)
  {
    return;
  }
  values.reserve(values.size() * srcValueCount);
  for (int i = 0; i < srcValueCount; ++i)
  {
    values.push_back(srcValues[i]);
  }
}

void AssimpMeshExporter::PushColor(std::vector<aiColor4D> &values,
                                   aiColor4D *srcValues,
                                   uint32_t srcValueCount)
{
  if (!srcValues)
  {
    return;
  }
  values.reserve(values.size() * srcValueCount);
  for (int i = 0; i < srcValueCount; ++i)
  {
    values.push_back(srcValues[i]);
  }
}


static csMatrix4f to_matrix4f(const aiMatrix4x4 &mat)
{
  return csMatrix4f(
      mat.a1, mat.b1, mat.c1, mat.d1,
      mat.a2, mat.b2, mat.c2, mat.d2,
      mat.a3, mat.b3, mat.c3, mat.d3,
      mat.a4, mat.b4, mat.c4, mat.d4
  );
}

static csVector3f to_vector(const aiVector3D &v)
{
  return csVector3f(v.x, v.y, v.z);
}

static csVector4f to_vector4(const aiVector3D &v)
{
  return csVector4f(v.x, v.y, v.z, 1.0f);
}


static aiVector3D to_vector(const csVector3f &v)
{
  return aiVector3D(v.x, v.y, v.z);
}

static aiVector3D to_vector3(const csVector4f &v)
{
  return aiVector3D(v.x, v.y, v.z);
}

struct MyWeight
{
  aiBone *bone;
  float  weight;
};

static std::vector<MyWeight> get_weights(const aiMesh *mesh, uint32_t vertexId, uint32_t maxInfluences)
{
  std::vector<MyWeight> weights;
  for (int              bi = 0; bi < mesh->mNumBones; ++bi)
  {
    aiBone   *bone = mesh->mBones[bi];
    for (int wi    = 0; wi < bone->mNumWeights; ++wi)
    {
      aiVertexWeight &weight = bone->mWeights[wi];
      if (weight.mVertexId == vertexId)
      {
        weights.emplace_back(bone, weight.mWeight);
      }
    }
  }

  std::sort(weights.begin(), weights.end(), [](const MyWeight &w0, const MyWeight &w1)
  {
    return w0.weight > w1.weight;
  });

  if (weights.size() > maxInfluences)
  {
    weights.resize(maxInfluences);
  }

  float     total_weight = 0.0f;
  for (auto &w: weights)
  {
    total_weight += w.weight;
  }
  for (auto &w: weights)
  {
    w.weight /= total_weight;
  }


  return weights;
}

void AssimpMeshExporter::ScanMatrices(const aiNode *node, const cs::csMatrix4f &parent)
{
  const csMatrix4f &mat   = to_matrix4f(node->mTransformation);
  const csMatrix4f global = parent * mat;
  m_matrices[node->mName.C_Str()] = global;

  for (int i = 0; i < node->mNumChildren; ++i)
  {
    ScanMatrices(node->mChildren[i], global);
  }
}

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

void AssimpMeshExporter::PushPosition(std::vector<aiVector3D> &values,
                                      const aiMatrix4x4 &matrix,
                                      const aiMesh *mesh,
                                      aiVector3D *srcValues,
                                      uint32_t srcValueCount)
{
  if (!srcValues)
  {
    return;
  }

//  min = {cs::csVector3f} {x=-0.388848543, y=-1.36245489, z=-0.419072032}
//  max = {cs::csVector3f} {x=0.382430077, y=0.280570477, z=0.571975946}
  if (mesh->HasBones() && false)
  {
    csMatrix4f skeletonMatrix = to_matrix4f(m_skeletonExporter.GetRootMatrix());

    values.resize(srcValueCount);
    
//    for (size_t iv = 0; iv<srcValueCount; iv++)
//    {
//      csVector4f meshVert = to_vector4(srcValues[iv]);
//      const std::vector<MyWeight> &weights = get_weights(mesh, iv, 4);
//      csVector4f v (0.0f, 0.0f, 0.0f, 0.0f);
//      for (const auto &w: weights)
//      {
//        aiBone *bone = w.bone;
//        csMatrix4f bonePoseMatrix   = to_matrix4f(bone->mOffsetMatrix);
//        csMatrix4f globalBoneMatrix = m_matrices[bone->mName.C_Str()];
//
//        v += (globalBoneMatrix * bonePoseMatrix * meshVert) * w.weight;
//      }
//      values[iv] = to_vector3(v);
//    }
    

    std::vector<int> numWeights;
    numWeights.resize(srcValueCount);
    std::vector<float> weightValues;
    weightValues.resize(srcValueCount);
    int max = 0;
    float maxWeight = 0.0f;
    for (int b = 0; b < mesh->mNumBones; ++b)
    {
      aiBone     *bone            = mesh->mBones[b];
      csMatrix4f bonePoseMatrix   = to_matrix4f(bone->mOffsetMatrix);
      csMatrix4f invBonePoseMatrix   = bonePoseMatrix.Inverted();
      csMatrix4f globalBoneMatrix = m_matrices[bone->mName.C_Str()];
      csMatrix4f tr = globalBoneMatrix * bonePoseMatrix;
      for (int   w                = 0; w < bone->mNumWeights; ++w)
      {
        aiVertexWeight &weight = bone->mWeights[w];
        if (weight.mVertexId < srcValueCount)
        {
          const csVector4f &meshVert = to_vector4(srcValues[weight.mVertexId]);
          csVector4f       v         = (tr * meshVert) * weight.mWeight;
          values[weight.mVertexId] += to_vector3(v);
          numWeights[weight.mVertexId]++;
          weightValues[weight.mVertexId] += weight.mWeight;
          max = MAX(max, numWeights[weight.mVertexId]);
          maxWeight = MAX(maxWeight, weightValues[weight.mVertexId]);
        }
      }

    }



  }
  else
  {
    values.reserve(srcValueCount);
    for (int i = 0; i < srcValueCount; ++i)
    {
      aiVector3D &srcV = srcValues[i];
      aiVector3D v     = matrix * srcV;
      values.push_back(v);
    }
  }
}


void AssimpMeshExporter::PushNormal(std::vector<aiVector3D> &values,
                                    const aiMatrix3x3 &matrix,
                                    aiVector3D *srcValues,
                                    uint32_t srcValueCount)
{
  if (!srcValues)
  {
    return;
  }
  values.reserve(values.size() * srcValueCount);
  for (int i = 0; i < srcValueCount; ++i)
  {
    aiVector3D v = matrix * srcValues[i];
    values.push_back(v);
  }
}

void AssimpMeshExporter::PushBones(std::vector<VertexBoneData> &boneData, aiBone *bone, uint32_t boneId)
{
  std::string boneName(bone->mName.C_Str());

  auto it = m_boneNames.find(boneName);
  if (it != m_boneNames.end())
  {
    boneId = it->second;
  }
  else
  {
    boneId = ++m_boneId;
    m_boneNames[boneName] = boneId;
  }
//  uint32_t    boneId = m_skeletonExporter.GetBoneIndex(boneName);

  for (int i = 0, in = bone->mNumWeights; i < in; ++i)
  {
    aiVertexWeight &weight = bone->mWeights[i];
    boneData[weight.mVertexId].weights.emplace_back(boneId, weight.mWeight);
  }
}

csVector3f vec(const aiVector3D &v)
{
  return csVector3f(v.x, v.y, v.z);
}


void AssimpMeshExporter::Push(cs::imp::AssimpMeshExporter::MeshData &meshData,
                              const aiMesh *mesh,
                              const aiMatrix4x4 &matrix)
{
  uint32_t indexOffset = meshData.position.size();
  uint32_t numVertices = mesh->mNumVertices;

  aiMatrix3x3 normalMatrix = aiMatrix3x3(aiMatrix4x4(matrix).Inverse().Transpose());

  aiMatrix4x4 id;

  PushPosition(meshData.position, matrix, mesh, mesh->mVertices, numVertices);

  csVector3f min(FLT_MAX, FLT_MAX, FLT_MAX);
  csVector3f max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
  for (auto  v: meshData.position)
  {
    min.x = MIN(min.x, v.x);
    min.y = MIN(min.y, v.y);
    min.z = MIN(min.z, v.z);
    max.x = MAX(max.x, v.x);
    max.y = MAX(max.y, v.y);
    max.z = MAX(max.z, v.z);
  }


  PushNormal(meshData.normal, normalMatrix, mesh->mNormals, numVertices);
  PushNormal(meshData.tangent, normalMatrix, mesh->mTangents, numVertices);
  PushColor(meshData.color0, mesh->mColors[0], numVertices);
  PushColor(meshData.color1, mesh->mColors[1], numVertices);
  PushTexCoord(meshData.texCoord0, mesh->mTextureCoords[0], numVertices);
  PushTexCoord(meshData.texCoord1, mesh->mTextureCoords[1], numVertices);
  PushTexCoord(meshData.texCoord2, mesh->mTextureCoords[2], numVertices);
  if (mesh->HasBones())
  {
    meshData.boneData.resize(meshData.position.size());
    if (VerifyMeshSkeletonMatchesBones(mesh))
    {
      for (uint32_t i = 0, in = mesh->mNumBones; i < in; i++)
      {
        PushBones(meshData.boneData, mesh->mBones[i], i);
      }
      NormalizeBoneWeights(meshData);
    }
  }


  meshData.indices.reserve(meshData.indices.size() + mesh->mNumFaces * 3);
  for (int i = 0; i < mesh->mNumFaces; ++i)
  {
    aiFace &face = mesh->mFaces[i];
    meshData.indices.push_back(indexOffset + face.mIndices[0]);
    meshData.indices.push_back(indexOffset + face.mIndices[1]);
    meshData.indices.push_back(indexOffset + face.mIndices[2]);
  }

}

bool AssimpMeshExporter::HasBoneData() const
{
  for (const auto &meshData: m_meshData)
  {
    if (!meshData.boneData.empty())
    {
      return true;
    }
  }
  return true;
}

void AssimpMeshExporter::SetSkeletonRootNames(const std::set<std::string> &rootNames)
{
  m_skeletonRootNames = rootNames;
}

aiNode *AssimpMeshExporter::FindSkeletonRootNode(aiNode *node)
{
  std::string nodeName(node->mName.C_Str());
  if (m_skeletonRootNames.contains(nodeName))
  {
    return node;
  }
  for (uint32_t i = 0, in = node->mNumChildren; i < in; i++)
  {
    aiNode *skelRoot = FindSkeletonRootNode(node->mChildren[i]);
    if (skelRoot)
    {
      return skelRoot;
    }
  }

  return nullptr;
}


bool AssimpMeshExporter::VerifyMeshSkeletonMatchesBones(const aiMesh *mesh)
{
  if (mesh->HasBones())
  {

    for (uint32_t i = 0, in = mesh->mNumBones; i < in; i++)
    {
      const aiBone *bone  = mesh->mBones[i];
      std::string  boneName(bone->mName.C_Str());
      uint32_t     boneId = m_skeletonExporter.GetBoneIndex(boneName);
      if (boneId == AssimpSkeletonExporter::IllegalBoneID)
      {
        return false;
      }
    }
  }
  return true;
}

void AssimpMeshExporter::NormalizeBoneWeights(cs::imp::AssimpMeshExporter::MeshData &meshData)
{
  for (auto &bd: meshData.boneData)
  {
    std::sort(bd.weights.begin(),
              bd.weights.end(),
              [](BoneWeight bw0, BoneWeight bw1)
              { return bw0.weight > bw1.weight; });

    // sum the weight and normalize it, so that the first 4 bone sum up to 1.0
    float         sum = 0.0f;
    for (uint32_t i   = 0, in = bd.weights.size(); i < in && i < 4; i++)
    {
      sum += bd.weights[i].weight;
    }
    if (sum != 0.0f)
    {
      for (uint32_t i = 0, in = bd.weights.size(); i < in && i < 4; i++)
      {
        bd.weights[i].weight /= sum;
      }
    }
  }
}


AssimpMeshExporter::VertexDecl AssimpMeshExporter::ExtractVertexDecl(const aiMesh *mesh)
{
  VertexDecl res;

  res.insert(VERTEX);
  if (mesh->mNormals)
  {
    res.insert(NORMAL);
  }
  if (mesh->mTangents)
  {
    res.insert(TANGENT);
  }
  if (mesh->mColors[0])
  {
    res.insert(COLOR0);
  }
  if (mesh->mColors[1])
  {
    res.insert(COLOR1);
  }
  if (mesh->mTextureCoords[0] && mesh->mNumUVComponents[0] == 1)
  {
    res.insert(TEX_COORD0_1);
  }
  if (mesh->mTextureCoords[0] && mesh->mNumUVComponents[0] == 2)
  {
    res.insert(TEX_COORD0_2);
  }
  if (mesh->mTextureCoords[0] && mesh->mNumUVComponents[0] == 3)
  {
    res.insert(TEX_COORD0_3);
  }
  if (mesh->mTextureCoords[1] && mesh->mNumUVComponents[1] == 1)
  {
    res.insert(TEX_COORD1_1);
  }
  if (mesh->mTextureCoords[1] && mesh->mNumUVComponents[1] == 2)
  {
    res.insert(TEX_COORD1_2);
  }
  if (mesh->mTextureCoords[1] && mesh->mNumUVComponents[1] == 3)
  {
    res.insert(TEX_COORD1_3);
  }
  if (mesh->mTextureCoords[2] && mesh->mNumUVComponents[2] == 1)
  {
    res.insert(TEX_COORD2_1);
  }
  if (mesh->mTextureCoords[2] && mesh->mNumUVComponents[2] == 2)
  {
    res.insert(TEX_COORD2_2);
  }
  if (mesh->mTextureCoords[2] && mesh->mNumUVComponents[2] == 3)
  {
    res.insert(TEX_COORD2_3);
  }
  if (mesh->HasBones())
  {
    res.insert(BONE_ID);
    res.insert(BONE_WEIGHT);
  }
  return res;
}

AssimpMeshExporter::MeshData &AssimpMeshExporter::FindMeshData(const VertexDecl &decl,
                                                               PrimitiveType primitiveType,
                                                               uint32_t materialIdx)
{
  for (auto &md: m_meshData)
  {
    if (md.materialIdx == materialIdx
        && md.vertexDecl == decl
        && md.primitiveType == primitiveType)
    {
      return md;
    }
  }

  MeshData md;
  md.vertexDecl    = decl;
  md.materialIdx   = materialIdx;
  md.primitiveType = primitiveType;

  if (decl.find(TEX_COORD0_1) != decl.end())
  {
    md.texCoord0Size = 1;
  }
  else if (decl.find(TEX_COORD0_2) != decl.end())
  {
    md.texCoord0Size = 2;
  }
  else if (decl.find(TEX_COORD0_3) != decl.end())
  {
    md.texCoord0Size = 3;
  }

  if (decl.find(TEX_COORD1_1) != decl.end())
  {
    md.texCoord1Size = 1;
  }
  else if (decl.find(TEX_COORD1_2) != decl.end())
  {
    md.texCoord1Size = 2;
  }
  else if (decl.find(TEX_COORD1_3) != decl.end())
  {
    md.texCoord1Size = 3;
  }

  if (decl.find(TEX_COORD2_1) != decl.end())
  {
    md.texCoord2Size = 1;
  }
  else if (decl.find(TEX_COORD2_2) != decl.end())
  {
    md.texCoord2Size = 2;
  }
  else if (decl.find(TEX_COORD2_3) != decl.end())
  {
    md.texCoord2Size = 3;
  }

  m_meshData.push_back(md);
  return m_meshData[m_meshData.size() - 1];
}


void
AssimpMeshExporter::WriteValuesV1(std::ostream &out, uint8_t dataType, const std::vector<aiVector3D> &vertices) const
{
  if (vertices.empty())
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = vertices.size() * sizeof(float) * 1;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (const auto &v: vertices)
  {
    out.write(reinterpret_cast<const char *>(&v), sizeof(float) * 1);
  }
}

void
AssimpMeshExporter::WriteValuesV2(std::ostream &out, uint8_t dataType, const std::vector<aiVector3D> &vertices) const
{
  if (vertices.empty())
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = vertices.size() * sizeof(float) * 2;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (const auto &v: vertices)
  {
    out.write(reinterpret_cast<const char *>(&v), sizeof(float) * 2);
  }
}


void
AssimpMeshExporter::WriteValuesV3(std::ostream &out, uint8_t dataType, const std::vector<aiVector3D> &vertices) const
{
  if (vertices.empty())
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = vertices.size() * sizeof(float) * 3;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (const auto &v: vertices)
  {
    out.write(reinterpret_cast<const char *>(&v), sizeof(float) * 3);
  }
}


void AssimpMeshExporter::WriteValuesC4(std::ostream &out, uint8_t dataType, const std::vector<aiColor4D> &colors) const
{
  if (colors.empty())
  {
    return;
  }
  uint8_t  type = dataType;
  uint32_t size = colors.size() * sizeof(float) * 4;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (const auto &v: colors)
  {
    out.write(reinterpret_cast<const char *>(&v), sizeof(float) * 4);
  }
}

void AssimpMeshExporter::WriteBoneWeights(std::ostream &out, const std::vector<VertexBoneData> &boneData) const
{
  if (boneData.empty())
  {
    return;
  }

  uint8_t  type = BONE_WEIGHT;
  uint32_t size = boneData.size() * sizeof(float) * 4;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (const auto &bone: boneData)
  {
    for (size_t i = 0; i < 4; i++)
    {
      float weight = i < bone.weights.size() ? bone.weights[i].weight : 0.0f;
      out.write(reinterpret_cast<char *>(&weight), sizeof(float));
    }
  }
}

void AssimpMeshExporter::WriteBoneIDs(std::ostream &out, const std::vector<VertexBoneData> &boneData) const
{
  if (boneData.empty())
  {
    return;
  }

  uint8_t  type = BONE_ID;
  uint32_t size = boneData.size() * sizeof(int32_t) * 4;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));
  out.write(reinterpret_cast<char *>(&size), sizeof(uint32_t));
  for (const auto &bone: boneData)
  {
    for (size_t i = 0; i < 4; i++)
    {
      uint32_t idx = i < bone.weights.size() ? bone.weights[i].boneId : 0;
      out.write(reinterpret_cast<char *>(&idx), sizeof(uint32_t));
    }
  }

}


void AssimpMeshExporter::Export(const std::string &filename, const std::string &referenceName) const
{
  csCryoFile file;

  bool hasBone = HasBoneData();

  csCryoFileElement *meshElement          = hasBone
                                            ? file.Root()->AddChild("skeletonMesh")
                                            : file.Root()->AddChild("mesh");
  csCryoFileElement *materialSlotsElement = meshElement->AddChild("materialSlots");
  csCryoFileElement *subMeshesElement     = meshElement->AddChild("subMeshes");

  uint32_t        idx = 0;
  for (const auto &meshData: m_meshData)
  {
    std::string idxStr            = std::to_string(idx);
    std::string dataName          = "#" + idxStr;
    std::string dataNameBoneIndex = "#SkelBoneIndex-" + idxStr;

    std::string materialSlotName = "Material_" + idxStr;
    std::string materialName     = "/materials/Default.mat";
    if (meshData.materialIdx < m_scene->mNumMaterials)
    {
      aiMaterial *material = m_scene->mMaterials[meshData.materialIdx];
      materialSlotName = material->GetName().C_Str();
      materialName     = referenceName + "_" + materialSlotName + ".matinstance";
    }

    csCryoFileElement *matSlotElement = materialSlotsElement->AddChild("materialSlot");
    matSlotElement->AddStringAttribute("name", materialSlotName);
    matSlotElement->AddStringAttribute("locator", materialName);


    csCryoFileElement *subMeshElement = subMeshesElement->AddChild("subMesh");
    subMeshElement->AddAttribute("slot", idxStr);
    subMeshElement->AddStringAttribute("dataIdx", dataName);


    {
      std::ostringstream ostream;
      WriteMesh(ostream, meshData);
      std::string dataStream = ostream.str();


      file.AddData(dataName,
                   dataStream.size(),
                   reinterpret_cast<uint8_t *>(dataStream.data()));
    }

    if (!meshData.boneData.empty())
    {
      subMeshElement->AddStringAttribute("boneId", dataNameBoneIndex);

      std::ostringstream ostream;
      WriteMeshBoneIndex(ostream, meshData);
      std::string dataStream = ostream.str();


      file.AddData(dataNameBoneIndex,
                   dataStream.size(),
                   reinterpret_cast<uint8_t *>(dataStream.data()));

      csCryoFileElement *boneMappingElement = subMeshElement->AddChild("boneMapping");
      for (auto e : m_boneNames)
      {
        csCryoFileElement *boneElement = boneMappingElement->AddChild("bone");
        boneElement->AddAttribute("idx", std::to_string(e.second));
        boneElement->AddStringAttribute("name", e.first);

      }
    }
    idx++;
  }

  std::ofstream out;
  out.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  file.Write(out, true, 2);
  out.close();
}

void AssimpMeshExporter::WriteMesh(std::ostream &out, const MeshData &meshData) const
{
  uint32_t version = 0x01;
  out.write(reinterpret_cast<char *>(&version), sizeof(uint32_t));

  uint32_t meshType = 0x01;
  if (!meshData.boneData.empty())
  {
    meshType = 0x02;
  }
  out.write(reinterpret_cast<char *>(&meshType), sizeof(uint32_t));


  uint32_t numVertices = meshData.position.size();
  out.write(reinterpret_cast<char *>(&numVertices), sizeof(uint32_t));

  WriteValuesV3(out, VERTEX, meshData.position);
  WriteValuesV3(out, NORMAL, meshData.normal);
  WriteValuesV3(out, TANGENT, meshData.tangent);
  WriteValuesC4(out, COLOR0, meshData.color0);
  WriteValuesC4(out, COLOR1, meshData.color1);

  switch (meshData.texCoord0Size)
  {
    case 1:
      WriteValuesV1(out, TEX_COORD0_1, meshData.texCoord0);
      break;
    case 2:
      WriteValuesV2(out, TEX_COORD0_2, meshData.texCoord0);
      break;
    case 3:
      WriteValuesV3(out, TEX_COORD0_3, meshData.texCoord0);
      break;
    default:
      break;
  }
  switch (meshData.texCoord1Size)
  {
    case 1:
      WriteValuesV1(out, TEX_COORD1_1, meshData.texCoord1);
      break;
    case 2:
      WriteValuesV2(out, TEX_COORD1_2, meshData.texCoord1);
      break;
    case 3:
      WriteValuesV3(out, TEX_COORD1_3, meshData.texCoord1);
      break;
    default:
      break;
  }
  switch (meshData.texCoord2Size)
  {
    case 1:
      WriteValuesV1(out, TEX_COORD2_1, meshData.texCoord2);
      break;
    case 2:
      WriteValuesV2(out, TEX_COORD2_2, meshData.texCoord2);
      break;
    case 3:
      WriteValuesV3(out, TEX_COORD2_3, meshData.texCoord2);
      break;
    default:
      break;
  }

  WriteBoneIDs(out, meshData.boneData);
  WriteBoneWeights(out, meshData.boneData);

  uint8_t type = END;
  out.write(reinterpret_cast<char *>(&type), sizeof(uint8_t));


  uint8_t primType = TRIANGLES;
  out.write(reinterpret_cast<char *>(&primType), sizeof(uint8_t));

  uint32_t numIndex = meshData.indices.size();
  out.write(reinterpret_cast<char *>(&numIndex), sizeof(uint32_t));


  if (numVertices >= 65336)
  {
    uint8_t indexType = UINT32;
    out.write(reinterpret_cast<char *>(&indexType), sizeof(uint8_t));
    uint32_t indexSize = numIndex * sizeof(uint32_t);
    out.write(reinterpret_cast<char *>(&indexSize), sizeof(uint32_t));
    for (auto i: meshData.indices)
    {
      uint32_t idx = i;
      out.write(reinterpret_cast<char *>(&idx), sizeof(uint32_t));
    }
  }
  else
  {
    uint8_t indexType = UINT16;
    out.write(reinterpret_cast<char *>(&indexType), sizeof(uint8_t));
    uint32_t indexSize = numIndex * sizeof(uint16_t);
    out.write(reinterpret_cast<char *>(&indexSize), sizeof(uint32_t));
    for (auto i: meshData.indices)
    {
      uint16_t idx = i;
      out.write(reinterpret_cast<char *>(&idx), sizeof(uint16_t));
    }
  }

}

void AssimpMeshExporter::WriteMeshBoneIndex(std::ostream &out, const cs::imp::AssimpMeshExporter::MeshData &mesh) const
{
  uint32_t version = 0x01;
  out.write(reinterpret_cast<char *>(&version), sizeof(uint32_t));

  WriteBoneIDs(out, mesh.boneData);

}


}