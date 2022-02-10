#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/material/ematerialattributetype.hh>
#include <ceCore/resource/iassetloader.hh>

namespace ce
{

struct iMaterial;
class Material;
class MaterialInstance;

CE_CLASS()
class CE_CORE_API MaterialLoaderCEF : public CE_SUPER(iAssetLoaderCEF)
{
  CE_CLASS_GEN_OBJECT;
public:
  virtual ~MaterialLoaderCEF() { }


  bool CanLoad(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

  iObject* Load(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

private:
  static Material* LoadMaterial(const Class* cls, const file::File* file, const ResourceLocator* locator);
  static iObject* LoadMaterialInstance(const Class* cls, const file::File* file, const ResourceLocator* locator);

  static void LoadShading(Material* material, const file::Element* materialElement);
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