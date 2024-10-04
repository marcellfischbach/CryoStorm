

#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csTypes.hh>
#include <csCore/graphics/eRenderPass.hh>
#include <csCore/graphics/iTexture.hh>
#include <csCore/graphics/material/eMaterialAttributeType.hh>
#include "csMaterial.hh"
#include <csCore/graphics/material/iMaterial.hh>
#include <csCore/math/csMatrix.hh>
#include <csCore/math/csVector.hh>
#include <string>

namespace cryo
{

class csMaterial;

CS_CLASS()
class CS_CORE_API csMaterialInstance : public CS_SUPER(iMaterial)
{
  CS_CLASS_GEN_OBJECT;
public:

  csMaterialInstance();
  virtual ~csMaterialInstance();

  const iShader *GetShader(eRenderPass pass) const override;

  eFillMode GetFillMode () const override;
  eRenderQueue GetRenderQueue() const override;
  eShadingMode GetShadingMode() const override;

  bool Bind(iDevice * device, eRenderPass pass) override;


  void SetMaterial(csMaterial * material);
  csMaterial* GetMaterial();
  const csMaterial* GetMaterial() const;

  Size IndexOf(const std::string & attributeName) override;

  void Set(Size idx, float value) override;
  void Set(Size idx, const csVector2f & v) override;
  void Set(Size idx, const csVector3f & v) override;
  void Set(Size idx, const csVector4f & v) override;
  void Set(Size idx, const csColor4f& v) override;
  void Set(Size idx, int value) override;
  void Set(Size idx, const csMatrix3f & m) override;
  void Set(Size idx, const csMatrix4f & m) override;
  void Set(Size idx, iTexture * texture) override;

  bool IsOverridden(Size idx) const;
  void SetOverride(Size idx, bool _override);
private:

  void RebuildAttributes();

  struct Attribute
  {
    bool Override;
    std::array<float, 16> Floats;
    std::array<int, 4> Ints;
    iTexture* Texture;
  };


  csMaterial * m_material;
  std::vector<Attribute> m_attributes;
};


}