//
// Created by MCEL on 24.02.2025.
//

#include <csCore/loaders/csRenderMeshLoader.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/csObjectRegistry.hh>

namespace cs
{

csRenderMeshLoader::csRenderMeshLoader()
{
  RegisterType("RMESH");
  RegisterType("RENDERMESH");
}


enum VDataType
{
  END,
  VERTEX,
  NORMAL,
  TANGENT,
  COLOR0,
  COLOR1,
  TEX_COORD0_1,
  TEX_COORD0_2,
  TEX_COORD0_3,
  TEX_COORD1_1,
  TEX_COORD1_2,
  TEX_COORD1_3,
  TEX_COORD2_1,
  TEX_COORD2_2,
  TEX_COORD2_3
};

uint32_t calc_byte_size(const std::vector<VDataType> &types)
{
  uint32_t        size = 0;
  for (auto item: types)
  {
    switch (item)
    {
      case VERTEX:
      case NORMAL:
      case TANGENT:
        size += 3 * sizeof(float);
        break;
      case COLOR0:
      case COLOR1:
        size += sizeof(uint32_t);
        break;
      case TEX_COORD0_1:
      case TEX_COORD1_1:
      case TEX_COORD2_1:
        size += sizeof(float);
        break;
      case TEX_COORD0_2:
      case TEX_COORD1_2:
      case TEX_COORD2_2:
        size += 2 * sizeof(float);
        break;
      case TEX_COORD0_3:
      case TEX_COORD1_3:
      case TEX_COORD2_3:
        size += 3 * sizeof(float);
        break;
      default:
        break;

    }
  }

  return size;
}


csOwned<iAsset> csRenderMeshLoader::Load(const csAssetLocator &locator) const
{
  csRef<iFile> file = csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!file)
  {
    return csOwned<iAsset>();
  }

  struct Header
  {
    uint32_t magic   = 0;
    uint32_t version = 0;
  };
  Header       header;
  file->Read(sizeof(Header), 1, &header);
  if (header.magic != 0x12341234 || header.version != 1)
  {
    return csOwned<iAsset>();
  }

  std::vector<VDataType> vertexStreams;
  while (true)
  {
    uint8_t type;
    file->Read(sizeof(uint8_t), 1, &type);
    if (type == 0)
    {
      break;
    }

    vertexStreams.push_back((VDataType) type);
  }


  uint32_t numVertices;
  file->Read(sizeof(uint32_t), 1, &numVertices);

  std::vector<csVector3f> vertices;
  std::vector<csVector3f> normals;
  std::vector<csVector3f> tangents;
  std::vector<csColor4f> colors;
  std::vector<csVector2f> uv0_2;
  std::vector<csVector3f> uv0_3;
  std::vector<csVector2f> uv1_2;

  csRef<iRenderMeshGenerator> generator = csObjectRegistry::Get<iRenderMeshGeneratorFactory>()->Create();

  for (uint32_t i=0; i<numVertices; i++)
  {
    for (auto type: vertexStreams)
    {
      float      v1;
      csVector2f v2;
      csVector3f v3;
      csColor4f col;
      switch (type) {
        case VERTEX:
          file->Read(sizeof(csVector3f), 1, &v3);
          vertices.push_back(v3);
          break;
        case NORMAL:
          file->Read(sizeof(csVector3f), 1, &v3);
          normals.push_back(v3);
          break;
        case TANGENT:
          file->Read(sizeof(csVector3f), 1, &v3);
          tangents.push_back(v3);
          break;
        case COLOR0:
          file->Read(sizeof(csColor4f), 1, &col);
          colors.push_back(col);
          break;
        case COLOR1:
          file->Read(sizeof(csColor4f), 1, &col);
          break;

        case TEX_COORD0_1:
          file->Read(sizeof(float), 1, &v1);
          break;
        case TEX_COORD0_2:
          file->Read(sizeof(csVector2f), 1, &v2);
          uv0_2.push_back(v2);
          break;
        case TEX_COORD0_3:
          file->Read(sizeof(csVector3f), 1, &v3);
          uv0_3.push_back(v3);
          break;

        case TEX_COORD1_1:
          file->Read(sizeof(float), 1, &v1);
          break;
        case TEX_COORD1_2:
          file->Read(sizeof(csVector2f), 1, &v2);
          uv1_2.push_back(v2);
          break;
        case TEX_COORD1_3:
          file->Read(sizeof(csVector3f), 1, &v3);
          break;

        case TEX_COORD2_1:
          file->Read(sizeof(float), 1, &v1);
          break;
        case TEX_COORD2_2:
          file->Read(sizeof(csVector2f), 1, &v2);
          break;
        case TEX_COORD2_3:
          file->Read(sizeof(csVector3f), 1, &v3);
          break;
      }
    }
  }

  for (auto type: vertexStreams)
  {
    float      v1;
    csVector2f v2;
    csVector3f v3;
    csColor4f  col;
    switch (type)
    {
      case VERTEX:
        generator->SetVertices(vertices);
        break;
      case NORMAL:
        generator->SetNormals(normals);
       break;
      case TANGENT:
        generator->SetTangents(tangents);
        break;
      case COLOR0:
        generator->SetColors(colors);
        break;

      case TEX_COORD0_2:
        generator->SetUV0(uv0_2);
        break;
      case TEX_COORD0_3:
        generator->SetUV0(uv0_3);
        break;

      case TEX_COORD1_2:
        generator->SetUV1(uv1_2);
        break;
      default:
        break;
    }
  }


  uint32_t numFaces;
  std::vector<uint32_t> indices;
  file->Read(sizeof(uint32_t), 1, &numFaces);
  for (uint32_t i=0; i<numFaces; i++)
  {
    if (numVertices >= 65536)
    {
      uint16_t vi[3] = {};
      file->Read(sizeof(uint16_t), 3, vi);
      indices.push_back(vi[0]);
      indices.push_back(vi[1]);
      indices.push_back(vi[2]);
    }
    else
    {
      uint32_t vi[3] = {};
      file->Read(sizeof(uint32_t), 3, vi);
      indices.push_back(vi[0]);
      indices.push_back(vi[1]);
      indices.push_back(vi[2]);
    }
  }
  file->Close();

  generator->SetIndices(indices);


  return generator->Generate();
}
} // cs