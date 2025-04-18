
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


CS_CLASS(jclass = "org.cryo.core.graphics.CsMesh")
class CS_CORE_API csMesh : public CS_SUPER(iAsset)
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
    SubMesh(const SubMesh &mesh);
    ~SubMesh();

    void SetMesh(iRenderMesh *mesh);
    iRenderMesh *GetMesh() const;

    void SetMaterialSlotIdx(Size materialSlotIdx);
    size_t GetMaterialSlotIdx() const;

    SubMesh &operator=(const SubMesh &mesh);

  protected:
    csRef<iRenderMesh> m_mesh;

    size_t m_materialSlotIdx;

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

    const std::string &GetName() const;

  private:
    csAssetRef<iMaterial> m_defaultMaterial;
    std::string           m_name;
  };

  const csBoundingBox &GetBoundingBox() const;

  CS_FUNCTION()
  size_t AddMaterialSlot(const std::string &name, cs::iMaterial *defaultMaterial = nullptr);
  CS_FUNCTION()
  void SetDefaultMaterial(size_t idx, cs::iMaterial *defaultMaterial);
  CS_FUNCTION()
  size_t GetNumberOfMaterialSlots() const;
  const MaterialSlot &GetMaterialSlot(size_t idx) const;
  CS_FUNCTION()
  size_t IndexOfMaterialSlot(const std::string &materialName) const;


  CS_FUNCTION()
  size_t AddSubMesh(cs::iRenderMesh *mesh, size_t materialSlotIdx);
  CS_FUNCTION()
  size_t GetNumberOfSubMeshes() const;
   SubMesh &GetSubMesh(Size idx) ;
  const SubMesh &GetSubMesh(Size idx) const;


protected:
  virtual SubMesh* CreateSubMesh ();

  csBoundingBox             m_boundingBox;
  std::vector<MaterialSlot> m_materialSlots;
  std::vector<SubMesh*>      m_subMeshes;


};


}