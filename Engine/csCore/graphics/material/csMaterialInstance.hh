

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

namespace cs
{

class csMaterial;

CS_CLASS(jclass = "org.cryo.core.graphics.material.MaterialInstance")
class CS_CORE_API csMaterialInstance : public CS_SUPER(iMaterial)
{
CS_CLASS_GEN_OBJECT;
CS_ASSET_GEN;
public:

  csMaterialInstance();
  ~csMaterialInstance() override;

  CS_FUNCTION(jenum)
  const cs::iShader *GetShader(cs::eRenderPass pass) const override;

  CS_FUNCTION(jenum)
  cs::eFillMode GetFillMode() const override;
  CS_FUNCTION(jenum)
  cs::eRenderQueue GetRenderQueue() const override;
  CS_FUNCTION(jenum)
  cs::eShadingMode GetShadingMode() const override;

  CS_FUNCTION(jenum = "cs::eRenderPass")
  bool Bind(cs::iDevice *device, cs::eRenderPass pass) override;


  CS_FUNCTION()
  void SetMaterial(cs::csMaterial *material);
  CS_FUNCTION()
  cs::csMaterial *GetMaterial();
  const cs::csMaterial *GetMaterial() const;

  CS_FUNCTION()
  size_t IndexOf(const std::string &attributeName) override;

  CS_FUNCTION()
  void SetFloat(size_t idx, float value) override;
  CS_FUNCTION()
  void SetVector2f(size_t idx, const cs::csVector2f &v) override;
  CS_FUNCTION()
  void SetVector3f(size_t idx, const cs::csVector3f &v) override;
  CS_FUNCTION()
  void SetVector4f(size_t idx, const cs::csVector4f &v) override;
  CS_FUNCTION()
  void SetColor4f(size_t idx, const cs::csColor4f &v) override;
  CS_FUNCTION()
  void SetInt(size_t idx, int value) override;
  void SetMatrix3f(size_t idx, const cs::csMatrix3f &m) override;
  void SetMatrix4f(size_t idx, const cs::csMatrix4f &m) override;
  CS_FUNCTION()
  void SetTexture(size_t idx, cs::iTexture *texture) override;

  CS_FUNCTION()
  bool IsOverridden(size_t idx) const;
  CS_FUNCTION()
  void SetOverride(size_t idx, bool _override);
private:

  void RebuildAttributes();

  struct Attribute
  {
    bool                  Override;
    std::array<float, 16> Floats;
    std::array<int, 4>    Ints;
    csAssetRef<iTexture>  Texture;
  };


  csAssetRef<csMaterial> m_material;
  std::vector<Attribute> m_attributes;
};


}