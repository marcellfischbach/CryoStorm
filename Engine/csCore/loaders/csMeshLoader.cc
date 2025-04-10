//
// Created by Marcell on 27.02.2025.
//

#include <csCore/loaders/csMeshLoader.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/graphics/csSkeletonMesh.hh>
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
  const csCryoFileElement *root = file->Root();
  if (root->GetNumberOfChildren() == 0)
  {
    return nullptr;
  }

  const csCryoFileElement *meshElement = file->Root()->GetChild(0);
  if (!meshElement)
  {
    return nullptr;
  }

  if (meshElement->GetTagName() == "mesh")
  {
    return LoadMesh(file, meshElement, locator);
  }
  else if (meshElement->GetTagName() == "skeletonMesh")
  {
    return LoadSkeletonMesh(file, meshElement, locator);
  }

  return nullptr;
}

csOwned<iAsset> csMeshLoader::LoadMesh(const file::csCryoFile *file,
                                       const file::csCryoFileElement *meshElement,
                                       const cs::csAssetLocator &locator) const
{
  const csCryoFileElement *materialSlotsElement = meshElement->GetChild("materialSlots");
  const csCryoFileElement *subMeshesElement     = meshElement->GetChild("subMeshes");
  if (!materialSlotsElement || !subMeshesElement)
  {
    return csOwned<iAsset>();
  }

  csAssetManager *man = csAssetManager::Get();

  csMesh   *mesh = new csMesh();
  for (int i     = 0; i < materialSlotsElement->GetNumberOfChildren(); ++i)
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
    const csCryoFileElement *subMeshElement = subMeshesElement->GetChild(i);
    if (subMeshElement->GetTagName() != "subMesh")
    {
      continue;
    }

    int slot = subMeshElement->GetAttribute("slot", 0);
    if (subMeshElement->HasAttribute("locator"))
    {
      std::string        meshLocStr = subMeshElement->GetAttribute("locator", "/narf");
      csRef<iRenderMesh> rmesh      = man->Get<iRenderMesh>(csAssetLocator(locator, meshLocStr));
      mesh->AddSubMesh(rmesh, slot);
    }
    else if (subMeshElement->HasAttribute("dataIdx"))
    {
      auto data = file->GetData(subMeshElement->GetAttribute("dataIdx", ""));
      if (!data.empty())
      {
        csBinaryInputStream is(data);
        csRef<iRenderMesh>  rmesh = ReadRenderMesh(is);
        mesh->AddSubMesh(rmesh, slot);
      }
    }

  }


  return csOwned<csMesh>(mesh);
}


csOwned<iAsset> csMeshLoader::LoadSkeletonMesh(const file::csCryoFile *file,
                                               const file::csCryoFileElement *meshElement,
                                               const cs::csAssetLocator &locator) const
{
  const csCryoFileElement *materialSlotsElement = meshElement->GetChild("materialSlots");
  const csCryoFileElement *subMeshesElement     = meshElement->GetChild("subMeshes");
  if (!materialSlotsElement || !subMeshesElement)
  {
    return csOwned<iAsset>();
  }

  csAssetManager *man = csAssetManager::Get();

  csSkeletonMesh   *mesh = new csSkeletonMesh();
  for (int i     = 0; i < materialSlotsElement->GetNumberOfChildren(); ++i)
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
    const csCryoFileElement *subMeshElement = subMeshesElement->GetChild(i);
    if (subMeshElement->GetTagName() != "subMesh")
    {
      continue;
    }

    csSkeletonMesh::SubMesh* subMesh = nullptr;
    int slot = subMeshElement->GetAttribute("slot", 0);
    if (subMeshElement->HasAttribute("locator"))
    {
      std::string        meshLocStr = subMeshElement->GetAttribute("locator", "/narf");
      csRef<iRenderMesh> rmesh      = man->Get<iRenderMesh>(csAssetLocator(locator, meshLocStr));
      size_t             idx        = mesh->AddSubMesh(rmesh, slot);
      subMesh = &static_cast<csSkeletonMesh::SubMesh&>(mesh->GetSubMesh(idx));
    }
    else if (subMeshElement->HasAttribute("dataIdx"))
    {
      auto data = file->GetData(subMeshElement->GetAttribute("dataIdx", ""));
      if (!data.empty())
      {
        csBinaryInputStream is(data);
        csRef<iRenderMesh>  rmesh = ReadRenderMesh(is);
        size_t              idx   = mesh->AddSubMesh(rmesh, slot);
        subMesh = &static_cast<csSkeletonMesh::SubMesh&>(mesh->GetSubMesh(idx));
      }
    }

    if (subMesh)
    {
      if (subMeshElement->HasAttribute("boneId"))
      {
        auto data = file->GetData(subMeshElement->GetAttribute("boneId", ""));
        if (!data.empty())
        {
          csBinaryInputStream is(data);
          std::vector<csVector4i> boneIndices = ReadBoneIndices(is);
          subMesh->SetOriginBoneIndices(boneIndices);
        }
      }

      const csCryoFileElement *boneMappingElement = subMeshElement->GetChild("boneMapping");
      if (boneMappingElement)
      {
        for (int m = 0; m < boneMappingElement->GetNumberOfChildren(); ++m)
        {
          const csCryoFileElement *boneElement = boneMappingElement->GetChild(m);
          if (boneElement->HasAttribute("idx")  && boneElement->HasAttribute("name"))
          {
            int idx = boneElement->GetAttribute("idx", -1);
            std::string name = boneElement->GetAttribute("name", "");
            if (idx != -1 && !name.empty())
            {
              subMesh->AddBone(idx, name);
            }
          }
        }
      }
    }

  }


  return csOwned<csMesh>(mesh);
}


csOwned<iRenderMesh> csMeshLoader::ReadRenderMesh(csBinaryInputStream &is) const
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
  BONE_ID,
  BONE_WEIGHT,
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
std::vector<T> read_v(csBinaryInputStream &is, uint32_t numValues)
{
  std::vector<T> values;
  values.resize(numValues);

  uint32_t size = is.Read<uint32_t>();
  is.Read(reinterpret_cast<uint8_t *>(values.data()), numValues * sizeof(T));
  return values;
}


csOwned<iRenderMesh> csMeshLoader::ReadRenderMesh_V1(csBinaryInputStream &is) const
{
  uint32_t meshType = is.Read<uint32_t>();
  // meshType is currently ignored

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
      case BONE_ID:
        generator->SetBoneIndices(read_v<csVector4i>(is, numVertices));
        break;
      case BONE_WEIGHT:
        generator->SetBoneWeights(read_v<csVector4f>(is, numVertices));
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


  uint8_t  primtiveType = is.Read<uint8_t>();
  uint32_t numIndices   = is.Read<uint32_t>();
  uint8_t  indexType    = is.Read<uint8_t>();


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



std::vector<csVector4i> csMeshLoader::ReadBoneIndices(cs::csBinaryInputStream &is) const
{
  uint32_t version = is.Read<uint32_t>();
  if (version == 1)
  {
    return ReadBoneIndices_V1(is);
  }
  return std::vector<csVector4i>();
}

std::vector<csVector4i> csMeshLoader::ReadBoneIndices_V1(cs::csBinaryInputStream &is) const
{
  uint8_t type = is.Read<uint8_t>();
  if (type != BONE_ID)
  {
    return std::vector<csVector4i>();
  }

  uint32_t size = is.Read<uint32_t>();
  if ((size % sizeof(csVector4i) != 0))
  {
    return std::vector<csVector4i>();
  };

  std::vector<csVector4i> boneIds;
  boneIds.resize(size / sizeof(csVector4i));
  is.Read(reinterpret_cast<uint8_t *>(boneIds.data()), size);

  return boneIds;
}

} // cs
