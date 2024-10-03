#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/material/ematerialattributetype.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace ce
{

struct iMaterial;
class Material;
class MaterialInstance;

CS_CLASS()
class CS_CORE_API MaterialLoader : public CS_SUPER(BaseCEFAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  MaterialLoader ();
  virtual ~MaterialLoader()
  {
  }


  iObject* Load(const CrimsonFile* file, const Class* cls, const ResourceLocator& locator) const override;

private:
  static Material* LoadMaterial(const Class* cls, const CrimsonFile* file, const ResourceLocator& locator);
  static iObject* LoadMaterialInstance(const Class* cls, const CrimsonFile* file, const ResourceLocator& locator);

  static void LoadShading(Material* material, const CrimsonFileElement* materialElement, const ResourceLocator& locator);
  static void LoadQueue(Material* material, const CrimsonFileElement* materialElement, const ResourceLocator& locator);
  static void LoadBlending(Material* material, const CrimsonFileElement* materialElement, const ResourceLocator& locator);
  static void LoadDepth(Material* material, const CrimsonFileElement* materialElement, const ResourceLocator& locator);

  static bool LoadShaders(Material* material, const CrimsonFileElement* materialElement, const ResourceLocator& locator);
  static bool LoadShader(Material* material, const CrimsonFileElement* shaderElement, const ResourceLocator& locator);
  static bool LoadAttributes(Material* material, const CrimsonFileElement* materialElement, const ResourceLocator& locator);
  static bool LoadAttribute(Material* material, const CrimsonFileElement* attributeElement, const ResourceLocator& locator);

  static bool LoadReferenceMaterial(MaterialInstance* materialInstance,
                                    const CrimsonFileElement* materialInstanceElement,
                                    const ResourceLocator& locator);
  static bool LoadAttributes(MaterialInstance* materialInstance,
                             const CrimsonFileElement* materialInstanceElement,
                             const ResourceLocator& locator);
  static bool LoadAttribute(MaterialInstance* materialInstance,
                            const CrimsonFileElement* attributeElement,
                            const ResourceLocator& locator);

  static eMaterialAttributeType GetAttributeType(const CrimsonFileElement* attributeElement);
  static bool LoadAttributeDefault(iMaterial* material,
                                   size_t attributeIdx,
                                   eMaterialAttributeType attributeType,
                                   const CrimsonFileElement* attributeElement,
                                   const ResourceLocator& locator);
  static bool LoadAttributeFloat(iMaterial* material, size_t attributeIdx, const CrimsonFileElement* attributeElement);
  static bool LoadAttributeVec2(iMaterial* material, size_t attributeIdx, const CrimsonFileElement* attributeElement);
  static bool LoadAttributeVec3(iMaterial* material, size_t attributeIdx, const CrimsonFileElement* attributeElement);
  static bool LoadAttributeVec4(iMaterial* material, size_t attributeIdx, const CrimsonFileElement* attributeElement);
  static bool LoadAttributeInt(iMaterial* material, size_t attributeIdx, const CrimsonFileElement* attributeElement);
  static bool LoadAttributeMatrix3(iMaterial* material, size_t attributeIdx, const CrimsonFileElement* attributeElement);
  static bool LoadAttributeMatrix4(iMaterial* material, size_t attributeIdx, const CrimsonFileElement* attributeElement);
  static bool LoadAttributeTexture(iMaterial* material,
                                   size_t attributeIdx,
                                   const CrimsonFileElement* attributeElement,
                                   const ResourceLocator& locator);
};

}