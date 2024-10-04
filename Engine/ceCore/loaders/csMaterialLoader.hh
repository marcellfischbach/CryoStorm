#pragma  once

#include <ceCore/csCoreExport.hh>
#include <ceCore/graphics/material/eMaterialAttributeType.hh>
#include <ceCore/resource/csBaseCSFAssetLoader.hh>

namespace cryo
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


  iObject* Load(const csCryoFile* file, const csClass* cls, const csResourceLocator& locator) const override;

private:
  static csMaterial* LoadMaterial(const csClass* cls, const csCryoFile* file, const csResourceLocator& locator);
  static iObject* LoadMaterialInstance(const csClass* cls, const csCryoFile* file, const csResourceLocator& locator);

  static void LoadShading(csMaterial* material, const csCryoFileElement* materialElement, const csResourceLocator& locator);
  static void LoadQueue(csMaterial* material, const csCryoFileElement* materialElement, const csResourceLocator& locator);
  static void LoadBlending(csMaterial* material, const csCryoFileElement* materialElement, const csResourceLocator& locator);
  static void LoadDepth(csMaterial* material, const csCryoFileElement* materialElement, const csResourceLocator& locator);

  static bool LoadShaders(csMaterial* material, const csCryoFileElement* materialElement, const csResourceLocator& locator);
  static bool LoadShader(csMaterial* material, const csCryoFileElement* shaderElement, const csResourceLocator& locator);
  static bool LoadAttributes(csMaterial* material, const csCryoFileElement* materialElement, const csResourceLocator& locator);
  static bool LoadAttribute(csMaterial* material, const csCryoFileElement* attributeElement, const csResourceLocator& locator);

  static bool LoadReferenceMaterial(csMaterialInstance* materialInstance,
                                    const csCryoFileElement* materialInstanceElement,
                                    const csResourceLocator& locator);
  static bool LoadAttributes(csMaterialInstance* materialInstance,
                             const csCryoFileElement* materialInstanceElement,
                             const csResourceLocator& locator);
  static bool LoadAttribute(csMaterialInstance* materialInstance,
                            const csCryoFileElement* attributeElement,
                            const csResourceLocator& locator);

  static eMaterialAttributeType GetAttributeType(const csCryoFileElement* attributeElement);
  static bool LoadAttributeDefault(iMaterial* material,
                                   size_t attributeIdx,
                                   eMaterialAttributeType attributeType,
                                   const csCryoFileElement* attributeElement,
                                   const csResourceLocator& locator);
  static bool LoadAttributeFloat(iMaterial* material, size_t attributeIdx, const csCryoFileElement* attributeElement);
  static bool LoadAttributeVec2(iMaterial* material, size_t attributeIdx, const csCryoFileElement* attributeElement);
  static bool LoadAttributeVec3(iMaterial* material, size_t attributeIdx, const csCryoFileElement* attributeElement);
  static bool LoadAttributeVec4(iMaterial* material, size_t attributeIdx, const csCryoFileElement* attributeElement);
  static bool LoadAttributeInt(iMaterial* material, size_t attributeIdx, const csCryoFileElement* attributeElement);
  static bool LoadAttributeMatrix3(iMaterial* material, size_t attributeIdx, const csCryoFileElement* attributeElement);
  static bool LoadAttributeMatrix4(iMaterial* material, size_t attributeIdx, const csCryoFileElement* attributeElement);
  static bool LoadAttributeTexture(iMaterial* material,
                                   size_t attributeIdx,
                                   const csCryoFileElement* attributeElement,
                                   const csResourceLocator& locator);
};

}