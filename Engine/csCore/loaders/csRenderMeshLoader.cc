//
// Created by MCEL on 24.02.2025.
//

#include <csCore/loaders/csRenderMeshLoader.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/resource/csAssetManager.hh>
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


enum IndexType
{
  UINT16,
  UINT32
};


template<typename T>
std::vector<T> read_v(iFile* file, uint32_t numValues)
{
  std::vector<T> values;
  values.resize(numValues);

  uint32_t size;
  file->Read(sizeof(uint32_t), 1, &size);
  file->Read(sizeof(T), numValues, values.data());
  return values;
}


csOwned<iAsset> csRenderMeshLoader::Load(const csAssetLocator &locator) const
{
  csRef<iFile> file = csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!file)
  {
    return csOwned<iAsset>();
  }


  const std::string &extension = locator.GetExtension();
  if (extension == "MESH")
  {


    return LoadMesh(file, locator);
  }
  else if (extension == "RMESH" || extension == "RENDERMESH")
  {




    return LoadRenderMesh(file);
  }
  return csOwned<iAsset>();
}

csOwned<iAsset> csRenderMeshLoader::LoadMesh(cs::iFile *file, const csAssetLocator &locator) const
{
  struct Header
  {
    uint32_t magic   = 0;
    uint32_t version = 0;
  };

  Header       header = file->Read<Header>();
  if (header.magic != 0x12341234 || header.version != 1)
  {
    return csOwned<iAsset>();
  }


  csRef<csMesh> mesh = new csMesh();
  
  uint32_t numMaterialSlots = file->Read<uint32_t>();
  for (uint32_t i=0; i<numMaterialSlots; i++)
  {
    std::string slotName = file->ReadString();
    std::string materialResource = file->ReadString();
    
    csAssetLocator materialLocator (locator, materialResource);
    csRef<iMaterial> material = csAssetManager::Get()->Get<iMaterial>(materialLocator);
    
    mesh->AddMaterialSlot(slotName, material);
  }
  
  
  uint32_t numSubMeshes = file->Read<uint32_t>();
  for (size_t i=0; i<numSubMeshes; i++)
  {
    uint32_t materialSlot = file->Read<uint32_t>();
    csRef<iRenderMesh> renderMesh = ReadRenderMesh(file);

    mesh->AddSubMesh(renderMesh, materialSlot);
  }




  return csOwned<iAsset>(mesh);
}

csOwned<iAsset> csRenderMeshLoader::LoadRenderMesh(cs::iFile *file) const
{
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

  return ReadRenderMesh(file);
}


csOwned<iRenderMesh> csRenderMeshLoader::ReadRenderMesh(cs::iFile *file) const
{
  csRef<iRenderMeshGenerator> generator = csObjectRegistry::Get<iRenderMeshGeneratorFactory>()->Create();


  uint32_t numVertices;
  file->Read(sizeof(uint32_t), 1, &numVertices);
  while (true)
  {
    uint8_t type;
    file->Read(sizeof(uint8_t), 1, &type);
    if (type == END)
    {
      break;
    }
    switch (type)
    {
      case VERTEX:
        generator->SetVertices(read_v<csVector3f>(file, numVertices));
        break;
      case NORMAL:
        generator->SetNormals(read_v<csVector3f>(file, numVertices));
        break;
      case TANGENT:
        generator->SetTangents(read_v<csVector3f>(file, numVertices));
        break;
      case COLOR0:
        generator->SetColors(read_v<csColor4f>(file, numVertices));
        break;
      case COLOR1:
        read_v<csColor4f>(file, numVertices);
        break;
      case TEX_COORD0_1:
        read_v<float>(file, numVertices);
        break;
      case TEX_COORD0_2:
        generator->SetUV0(read_v<csVector2f>(file, numVertices));
        break;
      case TEX_COORD0_3:
        generator->SetUV0(read_v<csVector3f>(file, numVertices));
        break;
      case TEX_COORD1_1:
        read_v<float>(file, numVertices);
        break;
      case TEX_COORD1_2:
        generator->SetUV1(read_v<csVector2f>(file, numVertices));
        break;
      case TEX_COORD1_3:
        read_v<csVector3f>(file, numVertices);
        break;
      case TEX_COORD2_1:
        read_v<float>(file, numVertices);
        break;
      case TEX_COORD2_2:
        generator->SetUV1(read_v<csVector2f>(file, numVertices));
        break;
      case TEX_COORD2_3:
        read_v<csVector3f>(file, numVertices);
        break;
      default:
        break;
    }

  }




  uint32_t numIndices;
  uint8_t primtiveType;
  uint8_t indexType;
  file->Read(sizeof(uint8_t), 1, &primtiveType);
  file->Read(sizeof(uint32_t), 1, &numIndices);
  file->Read(sizeof(uint8_t), 1, &indexType);

  generator->SetPrimitiveType(static_cast<ePrimitiveType>(primtiveType));
  switch (indexType)
  {
    case UINT16:
      generator->SetIndices(read_v<uint16_t>(file, numIndices));
      break;
    case UINT32:
      generator->SetIndices(read_v<uint32_t>(file, numIndices));
      break;
    default:
      break;
  }
  file->Close();


  return generator->Generate();
}
} // cs