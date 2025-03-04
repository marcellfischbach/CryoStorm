//
// Created by mcel on 04.03.2025.
//

#include <csAssimpImporter/AssimpMeshExporter.hh>
#include <csCryoFile/csCryoFile.hh>
#include <sstream>
#include <ostream>

using namespace cs::file;

namespace cs::imp
{

AssimpMeshExporter::AssimpMeshExporter()
{

}

void AssimpMeshExporter::combine(const aiScene *scene, const aiMesh *mesh)
{
  aiMatrix4x4 identity;
  PushMesh(identity, mesh);
}

void AssimpMeshExporter::combine(const aiScene *scene, const aiNode *node)
{
  aiMatrix4x4 root;
  combine(scene, node, root);
}


void AssimpMeshExporter::combine(const aiScene *scene, const aiNode *node, const aiMatrix4x4 &parent)
{
  aiMatrix4x4 mat = parent * node->mTransformation;
  for (int    i   = 0; i < node->mNumMeshes; ++i)
  {
    unsigned int meshId = node->mMeshes[i];
    if (meshId >= scene->mNumMeshes)
    {
      continue;
    }
    aiMesh *mesh = scene->mMeshes[meshId];
    if (!mesh)
    {
      continue;
    }

    PushMesh(mat, mesh);

  }

  for (int i = 0; i < node->mNumChildren; ++i)
  {
    aiNode *child = node->mChildren[i];
    combine(scene, child, mat);
  }
}

void AssimpMeshExporter::PushMesh(const aiMatrix4x4 &mat, const aiMesh *mesh)
{
  VertexDecl vertDecl = ExtractVertexDecl(mesh);
  MeshData   &data    = FindMeshData(vertDecl, TRIANGLES, mesh->mMaterialIndex);

  Push(data, mesh, mat);
}


static void push(std::vector<aiVector3D> &values,
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

static void push(std::vector<aiColor4D> &values,
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


static void push(std::vector<aiVector3D> &values,
                 const aiMatrix4x4 &matrix,
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
    aiVector3D &srcV = srcValues[i];
    aiVector3D v     = matrix * srcV;
    values.push_back(v);
  }
}


static void push(std::vector<aiVector3D> &values,
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


void AssimpMeshExporter::Push(cs::imp::AssimpMeshExporter::MeshData &meshData,
                              const aiMesh *mesh,
                              const aiMatrix4x4 &matrix)
{
  uint32_t indexOffset = meshData.position.size();
  uint32_t numVertices = mesh->mNumVertices;

  aiMatrix3x3 normalMatrix = aiMatrix3x3(aiMatrix4x4(matrix).Inverse().Transpose());


  push(meshData.position, matrix, mesh->mVertices, numVertices);
  push(meshData.normal, normalMatrix, mesh->mNormals, numVertices);
  push(meshData.tangent, normalMatrix, mesh->mTangents, numVertices);
  push(meshData.color0, mesh->mColors[0], numVertices);
  push(meshData.color1, mesh->mColors[1], numVertices);
  push(meshData.texCoord0, mesh->mTextureCoords[0], numVertices);
  push(meshData.texCoord1, mesh->mTextureCoords[1], numVertices);
  push(meshData.texCoord2, mesh->mTextureCoords[2], numVertices);


  meshData.indices.reserve(meshData.indices.size() + mesh->mNumFaces * 3);
  for (int i = 0; i < mesh->mNumFaces; ++i)
  {
    aiFace &face = mesh->mFaces[i];
    meshData.indices.push_back(indexOffset + face.mIndices[0]);
    meshData.indices.push_back(indexOffset + face.mIndices[1]);
    meshData.indices.push_back(indexOffset + face.mIndices[2]);
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


void write_values_v1(std::ostream &out, uint8_t dataType, const std::vector<aiVector3D> &vertices)
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

void write_values_v2(std::ostream &out, uint8_t dataType, const std::vector<aiVector3D> &vertices)
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


void write_values_v3(std::ostream &out, uint8_t dataType, const std::vector<aiVector3D> &vertices)
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


void write_values_c4(std::ostream &out, uint8_t dataType, const std::vector<aiColor4D> &colors)
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

void AssimpMeshExporter::Export(const std::string &filename) const
{
  csCryoFile file;

  csCryoFileElement *meshElement          = file.Root()->AddChild("mesh");
  csCryoFileElement *materialSlotsElement = meshElement->AddChild("materialSlots");
  csCryoFileElement *subMeshesElement     = meshElement->AddChild("subMeshes");

  uint32_t        idx = 0;
  for (const auto &meshData: m_meshData)
  {
    std::string idxStr = std::to_string(idx);
    std::string dataName = "#" + idxStr;


    csCryoFileElement *matSlotElement = materialSlotsElement->AddChild("materialSlot");
    matSlotElement->AddAttribute("name", "Material_" + idxStr);
    matSlotElement->AddStringAttribute("locator", "/materials/Default.mat");


    csCryoFileElement *subMeshElement = subMeshesElement->AddChild("subMesh");
    subMeshElement->AddAttribute("slot", idxStr);
    subMeshElement->AddStringAttribute("dataIdx", dataName);


    std::ostringstream ostream;
    WriteMesh(ostream, meshData);
    std::string dataStream = ostream.str();


    file.AddData(dataName,
                 dataStream.size(),
                 reinterpret_cast<uint8_t *>(dataStream.data()));
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


  uint32_t numVertices = meshData.position.size();
  out.write(reinterpret_cast<char *>(&numVertices), sizeof(uint32_t));

  write_values_v3(out, VERTEX, meshData.position);
  write_values_v3(out, NORMAL, meshData.normal);
  write_values_v3(out, TANGENT, meshData.tangent);
  write_values_c4(out, COLOR0, meshData.color0);
  write_values_c4(out, COLOR1, meshData.color1);

  switch (meshData.texCoord0Size)
  {
    case 1:
      write_values_v1(out, TEX_COORD0_1, meshData.texCoord0);
      break;
    case 2:
      write_values_v2(out, TEX_COORD0_2, meshData.texCoord0);
      break;
    case 3:
      write_values_v3(out, TEX_COORD0_3, meshData.texCoord0);
      break;
    default:
      break;
  }
  switch (meshData.texCoord1Size)
  {
    case 1:
      write_values_v1(out, TEX_COORD1_1, meshData.texCoord1);
      break;
    case 2:
      write_values_v2(out, TEX_COORD1_2, meshData.texCoord1);
      break;
    case 3:
      write_values_v3(out, TEX_COORD1_3, meshData.texCoord1);
      break;
    default:
      break;
  }
  switch (meshData.texCoord2Size)
  {
    case 1:
      write_values_v1(out, TEX_COORD2_1, meshData.texCoord2);
      break;
    case 2:
      write_values_v2(out, TEX_COORD2_2, meshData.texCoord2);
      break;
    case 3:
      write_values_v3(out, TEX_COORD2_3, meshData.texCoord2);
      break;
    default:
      break;
  }
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


}