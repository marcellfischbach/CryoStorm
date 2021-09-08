#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc
{

struct iMaterial;
class Material;

SPC_CLASS()
class SPC_CORE_API MaterialLoaderSpc : public SPC_SUPER(iAssetLoaderSpc)
{
  SPC_CLASS_GEN_OBJECT;
public:
  virtual ~MaterialLoaderSpc() { }


  bool CanLoad(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

  iObject* Load(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

private:
  Material* LoadMaterial(const Class* cls, const file::File* file, const ResourceLocator* locator) const;
  iObject* LoadMaterialInstance(const Class* cls, const file::File* file, const ResourceLocator* locator) const;


  bool LoadShaders(Material* material, const file::Element* materialElement) const;
  bool LoadShader(Material* material, const file::Element* shaderElement) const;
  bool LoadAttributes(Material* material, const file::Element* materialElement) const;
  bool LoadAttribute(Material* material, const file::Element* attributeElement) const;




  bool LoadAttributeDefault(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
  bool LoadAttributeFloat(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
  bool LoadAttributeVec2(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
  bool LoadAttributeVec3(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
  bool LoadAttributeVec4(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
  bool LoadAttributeColor4(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
  bool LoadAttributeInt(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
  bool LoadAttributeMatrix3(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
  bool LoadAttributeMatrix4(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
  bool LoadAttributeTexture(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement) const;
};


}