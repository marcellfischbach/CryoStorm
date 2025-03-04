//
// Created by Marcell on 27.02.2025.
//

#include <csCore/loaders/csMeshLoader.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csBinaryData.hh>
#include <csCore/csObjectRegistry.hh>

namespace cs
{

using namespace file;

csMeshLoader::csMeshLoader()
    : csBaseCSFAssetLoader()
{
  RegisterType("MESH");
}

csOwned<iAsset> csMeshLoader::Load(const csCryoFile *file, const csAssetLocator &locator) const
{
  const csCryoFileElement *meshElement = file->Root()->GetChild("mesh");
  if (!meshElement)
  {
    return csOwned<iAsset>();
  }

  const csCryoFileElement *materialSlotsElement = meshElement->GetChild("materialSlots");
  const csCryoFileElement *subMeshesElement     = meshElement->GetChild("subMeshes");
  if (!materialSlotsElement || !subMeshesElement)
  {
    return csOwned<iAsset>();
  }

  csAssetManager   *man               = csAssetManager::Get();

  csMesh *mesh = new csMesh();
  for (int i = 0; i < materialSlotsElement->GetNumberOfChildren(); ++i)
  {
    const csCryoFileElement *materialSlotElement = materialSlotsElement->GetChild(i);
    if (materialSlotElement->GetTagName() != "materialSlot")
    {
      continue;
    }

    std::string      slotName  = materialSlotElement->GetAttribute("name", "Def");
    std::string      matLocStr = materialSlotElement->GetAttribute("locator", "/narf");
    csRef<iMaterial> material  = man->Get<iMaterial>(csAssetLocator(locator, matLocStr));

    mesh->AddMaterialSlot(slotName, material);
  }

  for (int i = 0; i < subMeshesElement->GetNumberOfChildren(); ++i)
  {
    const csCryoFileElement *meshElement = subMeshesElement->GetChild(i);
    if (meshElement->GetTagName() != "subMesh")
    {
      continue;
    }

    int         slot              = meshElement->GetAttribute("slot", 0);
    if (meshElement->HasAttribute("locator"))
    {
        std::string meshLocStr        = meshElement->GetAttribute("locator", "/narf");
        csRef<iRenderMesh> rmesh = man->Get<iRenderMesh>(csAssetLocator(locator, meshLocStr));
        mesh->AddSubMesh(rmesh, slot);
    }
    else if (meshElement->HasAttribute("dataIdx"))
    {
      auto data = file->GetData(meshElement->GetAttribute("dataIdx", ""));
      if (!data.empty())
      {
        csBinaryInputStream is(data);
        csRef<iRenderMesh> rmesh = ReadRenderMesh(is);
        mesh->AddSubMesh(rmesh, slot);
      }
    }

  }


  return csOwned<csMesh>(mesh);
}


csOwned<iRenderMesh> csMeshLoader::ReadRenderMesh(csBinaryInputStream& is) const
{
  uint32_t version = is.Read<uint32_t>();
  if (version == 1)
  {
    return ReadRenderMesh_V1(is);
  }
  return csOwned<iRenderMesh>();
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
std::vector<T> read_v(csBinaryInputStream& is, uint32_t numValues)
{
  std::vector<T> values;
  values.resize(numValues);

  uint32_t size = is.Read<uint32_t>();
  is.Read(reinterpret_cast<uint8_t*>(values.data()), numValues * sizeof(T));
  return values;
}


csOwned<iRenderMesh> csMeshLoader::ReadRenderMesh_V1(csBinaryInputStream& is) const
{

  csRef<iRenderMeshGenerator> generator = csObjectRegistry::Get<iRenderMeshGeneratorFactory>()->Create();


  uint32_t numVertices = is.Read<uint32_t>();
  while (true)
  {
    uint8_t type = is.Read<uint8_t>();
    if (type == END)
    {
      break;
    }
    switch (type)
    {
    case VERTEX:
      generator->SetVertices(read_v<csVector3f>(is, numVertices));
      break;
    case NORMAL:
      generator->SetNormals(read_v<csVector3f>(is, numVertices));
      break;
    case TANGENT:
      generator->SetTangents(read_v<csVector3f>(is, numVertices));
      break;
    case COLOR0:
      generator->SetColors(read_v<csColor4f>(is, numVertices));
      break;
    case COLOR1:
      read_v<csColor4f>(is, numVertices);
      break;
    case TEX_COORD0_1:
      read_v<float>(is, numVertices);
      break;
    case TEX_COORD0_2:
      generator->SetUV0(read_v<csVector2f>(is, numVertices));
      break;
    case TEX_COORD0_3:
      generator->SetUV0(read_v<csVector3f>(is, numVertices));
      break;
    case TEX_COORD1_1:
      read_v<float>(is, numVertices);
      break;
    case TEX_COORD1_2:
      generator->SetUV1(read_v<csVector2f>(is, numVertices));
      break;
    case TEX_COORD1_3:
      read_v<csVector3f>(is, numVertices);
      break;
    case TEX_COORD2_1:
      read_v<float>(is, numVertices);
      break;
    case TEX_COORD2_2:
      generator->SetUV1(read_v<csVector2f>(is, numVertices));
      break;
    case TEX_COORD2_3:
      read_v<csVector3f>(is, numVertices);
      break;
    default:
      break;
    }

  }




  uint8_t primtiveType = is.Read<uint8_t>();
  uint32_t numIndices = is.Read<uint32_t>();
  uint8_t indexType = is.Read<uint8_t>();


  generator->SetPrimitiveType(static_cast<ePrimitiveType>(primtiveType));
  switch (indexType)
  {
  case UINT16:
    generator->SetIndices(read_v<uint16_t>(is, numIndices));
    break;
  case UINT32:
    generator->SetIndices(read_v<uint32_t>(is, numIndices));
    break;
  default:
    break;
  }


  return generator->Generate();
}

} // cs
