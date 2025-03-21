#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/material/eMaterialAttributeType.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>

namespace cs
{

struct iMaterial;
class csMaterial;
class csMaterialInstance;

CS_CLASS()
class CS_CORE_API csMaterialLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  csMaterialLoader();
  virtual ~csMaterialLoader()
  {
  }


  csOwned<iAsset> Load(const file::csCryoFile *file, const csAssetLocator &locator) const override;

private:
  static csOwned<csMaterial> LoadMaterial(const file::csCryoFile* file, const csAssetLocator& locator);
  static csOwned<csMaterialInstance> LoadMaterialInstance(const file::csCryoFile* file, const csAssetLocator& locator);

  static void LoadShading(csMaterial* material, const file::csCryoFileElement* materialElement, const csAssetLocator& locator);
  static void LoadQueue(csMaterial* material, const file::csCryoFileElement* materialElement, const csAssetLocator& locator);
  static void LoadBlending(csMaterial* material, const file::csCryoFileElement* materialElement, const csAssetLocator& locator);
  static void LoadDepth(csMaterial* material, const file::csCryoFileElement* materialElement, const csAssetLocator& locator);

  static bool LoadShaders(csMaterial* material, const file::csCryoFileElement* materialElement, const csAssetLocator& locator);
  static bool LoadShader(csMaterial* material, const file::csCryoFileElement* shaderElement, const csAssetLocator& locator);
  static bool LoadAttributes(csMaterial* material, const file::csCryoFileElement* materialElement, const csAssetLocator& locator);
  static bool LoadAttribute(csMaterial* material, const file::csCryoFileElement* attributeElement, const csAssetLocator& locator);

  static bool LoadReferenceMaterial(csMaterialInstance* materialInstance,
                                    const file::csCryoFileElement* materialInstanceElement,
                                    const csAssetLocator& locator);
  static bool LoadAttributes(csMaterialInstance* materialInstance,
                             const file::csCryoFileElement* materialInstanceElement,
                             const csAssetLocator& locator);
  static bool LoadAttribute(csMaterialInstance* materialInstance,
                            const file::csCryoFileElement* attributeElement,
                            const csAssetLocator& locator);

  static eMaterialAttributeType GetAttributeType(const file::csCryoFileElement* attributeElement);
  static bool LoadAttributeDefault(iMaterial* material,
                                   size_t attributeIdx,
                                   eMaterialAttributeType attributeType,
                                   const file::csCryoFileElement* attributeElement,
                                   const csAssetLocator& locator);
  static bool LoadAttributeFloat(iMaterial* material, size_t attributeIdx, const file::csCryoFileElement* attributeElement);
  static bool LoadAttributeVec2(iMaterial* material, size_t attributeIdx, const file::csCryoFileElement* attributeElement);
  static bool LoadAttributeVec3(iMaterial* material, size_t attributeIdx, const file::csCryoFileElement* attributeElement);
  static bool LoadAttributeVec4(iMaterial* material, size_t attributeIdx, const file::csCryoFileElement* attributeElement);
  static bool LoadAttributeInt(iMaterial* material, size_t attributeIdx, const file::csCryoFileElement* attributeElement);
  static bool LoadAttributeMatrix3(iMaterial* material, size_t attributeIdx, const file::csCryoFileElement* attributeElement);
  static bool LoadAttributeMatrix4(iMaterial* material, size_t attributeIdx, const file::csCryoFileElement* attributeElement);
  static bool LoadAttributeTexture(iMaterial* material,
                                   size_t attributeIdx,
                                   const file::csCryoFileElement* attributeElement,
                                   const csAssetLocator& locator);
};

}