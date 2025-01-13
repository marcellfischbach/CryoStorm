
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/graphics/material/iMaterial.hh>
#include <csCore/csRef.hh>
#include <csCore/csTypes.hh>


#include <string>
#include <vector>


namespace cs
{


CS_CLASS()
class CS_CORE_API csMesh : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csMesh();
  ~csMesh() override;



  class CS_CORE_API SubMesh
  {
    friend class csMesh;
  private:
  public:
    SubMesh();
    SubMesh(const SubMesh & mesh);
    ~SubMesh();

    void SetMesh(iRenderMesh* mesh);
    iRenderMesh* GetMesh() const;

    void SetMaterialSlotIdx(Size materialSlotIdx);
    Size GetMaterialSlotIdx() const;

    SubMesh &operator=(const SubMesh &mesh);

  private:
    iRenderMesh* m_mesh;

    Size m_materialSlotIdx;

  };


  class CS_CORE_API MaterialSlot
  {
    friend class csMesh;
  private:
    MaterialSlot(const std::string &name, iMaterial *defaultMaterial);
  public:
    MaterialSlot(const MaterialSlot &slot);
    ~MaterialSlot();

    iMaterial *GetDefaultMaterial() const;

    MaterialSlot &operator=(const MaterialSlot &slot);

    const std::string& GetName() const;

  private:
    csAssetRef<iMaterial> m_defaultMaterial;
    std::string           m_name;
  };

  const csBoundingBox& GetBoundingBox() const;

  Size AddMaterialSlot(const std::string& name, iMaterial *defaultMaterial = nullptr);
  void SetDefaultMaterial(Size idx, iMaterial *defaultMaterial);
  Size GetNumberOfMaterialSlots() const;
  const MaterialSlot& GetMaterialSlot(Size idx) const;
  size_t IndexOfMaterialSlot(const std::string &materialName) const;



  void AddSubMesh(iRenderMesh* mesh, Size materialSlotIdx);
  Size GetNumberOfSubMeshes() const;
  const SubMesh& GetSubMesh(Size idx) const;


private:
  csBoundingBox             m_boundingBox;
  std::vector<MaterialSlot> m_materialSlots;
  std::vector<SubMesh> m_subMeshes;


};


}