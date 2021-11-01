#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/material/ematerialattributetype.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc
{

struct iMaterial;
class Material;
class MaterialInstance;

SPC_CLASS()
class SPC_CORE_API MaterialLoaderSpc : public SPC_SUPER(iAssetLoaderSpc)
{
  SPC_CLASS_GEN_OBJECT;
public:
  virtual ~MaterialLoaderSpc() { }


  bool CanLoad(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

  iObject* Load(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

private:
  static Material* LoadMaterial(const Class* cls, const file::File* file, const ResourceLocator* locator);
  static iObject* LoadMaterialInstance(const Class* cls, const file::File* file, const ResourceLocator* locator);

  static void LoadQueue(Material* material, const file::Element* materialElement);
  static void LoadBlending(Material* material, const file::Element* materialElement);
  static void LoadDepth(Material* material, const file::Element* materialElement);

  static bool LoadShaders(Material* material, const file::Element* materialElement);
  static bool LoadShader(Material* material, const file::Element* shaderElement);
  static bool LoadAttributes(Material* material, const file::Element* materialElement);
  static bool LoadAttribute(Material* material, const file::Element* attributeElement);

  static bool LoadReferenceMaterial (MaterialInstance *materialInstance, const file::Element *materialInstanceElement);
  static bool LoadAttributes(MaterialInstance* materialInstance, const file::Element* materialInstanceElement);
  static bool LoadAttribute(MaterialInstance* materialInstance, const file::Element* attributeElement);


  static eMaterialAttributeType GetAttributeType (const file::Element *attributeElement);
  static bool LoadAttributeDefault(iMaterial* material, size_t attributeIdx, eMaterialAttributeType attributeType, const file::Element* attributeElement);
  static bool LoadAttributeFloat(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement);
  static bool LoadAttributeVec2(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement);
  static bool LoadAttributeVec3(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement);
  static bool LoadAttributeVec4(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement);
  static bool LoadAttributeInt(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement);
  static bool LoadAttributeMatrix3(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement);
  static bool LoadAttributeMatrix4(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement);
  static bool LoadAttributeTexture(iMaterial* material, size_t attributeIdx, const file::Element* attributeElement);
};


}