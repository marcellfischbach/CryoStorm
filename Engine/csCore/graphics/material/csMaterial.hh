

#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csTypes.hh>
#include <csCore/graphics/eBlendFactor.hh>
#include <csCore/graphics/eDepthMode.hh>
#include <csCore/graphics/eRenderPass.hh>
#include <csCore/graphics/eShadingMode.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iTexture.hh>
#include <csCore/graphics/material/eMaterialAttributeType.hh>
#include <csCore/graphics/material/iMaterial.hh>
#include <csCore/math/csColor4f.hh>
#include <csCore/math/csMatrix.hh>
#include <csCore/math/csVector.hh>
#include <string>
#include <array>

namespace cs
{

struct iShader;
struct iShaderAttribute;
struct iTexture;


CS_CLASS(jclass="org.cryo.core.graphics.material.Material")
class CS_CORE_API csMaterial : public CS_SUPER(iMaterial)
{
  friend class csMaterialInstance;
  CS_CLASS_GEN_OBJECT;
  CS_ASSET_GEN;
public:
  static const size_t UndefinedIndex = ~0x00;

  csMaterial();
  ~csMaterial() override;

  CS_FUNCTION(jenum="cs::eRenderPass")
  bool Bind(cs::iDevice * device, cs::eRenderPass pass) override;

  CS_FUNCTION(jenum)
  void SetFillMode (cs::eFillMode fillMode);
  CS_FUNCTION(jenum)
  CS_NODISCARD cs::eFillMode GetFillMode () const override;

  CS_FUNCTION(jenum)
  void SetRenderQueue(cs::eRenderQueue queue);
  CS_FUNCTION(jenum)
  CS_NODISCARD cs::eRenderQueue GetRenderQueue() const override;

  CS_FUNCTION()
  void SetBlending(bool  blending);
  CS_FUNCTION()
  CS_NODISCARD bool IsBlending () const;

  CS_FUNCTION(jenum="cs::eBlendFactor")
  void SetBlendFactor (cs::eBlendFactor srcFactor, cs::eBlendFactor dstFactor);
  CS_FUNCTION(jenum="cs::eBlendFactor")
  void SetBlendFactor (cs::eBlendFactor srcFactorColor, cs::eBlendFactor srcFactorAlpha, cs::eBlendFactor dstFactorColor, cs::eBlendFactor dstFactorAlpha);
  CS_FUNCTION(jenum)
  CS_NODISCARD cs::eBlendFactor GetBlendFactorSrcColor () const;
  CS_FUNCTION(jenum)
  CS_NODISCARD cs::eBlendFactor GetBlendFactorSrcAlpha () const;
  CS_FUNCTION(jenum)
  CS_NODISCARD cs::eBlendFactor GetBlendFactorDstColor () const;
  CS_FUNCTION(jenum)
  CS_NODISCARD cs::eBlendFactor GetBlendFactorDstAlpha () const;

  CS_FUNCTION()
  void SetDepthWrite (bool depthWrite);
  CS_FUNCTION()
  CS_NODISCARD bool IsDepthWrite() const;
  CS_FUNCTION()
  void SetDepthTest (bool depthTest);
  CS_FUNCTION()
  CS_NODISCARD bool IsDepthTest() const;
  CS_FUNCTION(jenum)
  void SetDepthFunc (cs::eCompareFunc depthFun);
  CS_FUNCTION(jenum)
  CS_NODISCARD cs::eCompareFunc GetDepthFunc() const;
  CS_FUNCTION(jenum)
  void SetShadingMode (cs::eShadingMode shadingMode);
  CS_FUNCTION(jenum)
  CS_NODISCARD cs::eShadingMode GetShadingMode () const override;


  CS_FUNCTION(jenum="cs::eRenderPass")
  void SetShader(cs::eRenderPass pass, cs::iShader *shader);
  CS_FUNCTION(jenum="cs::eRenderPass")
  CS_NODISCARD cs::iShader *GetShader(cs::eRenderPass pass);
  CS_NODISCARD const cs::iShader *GetShader(eRenderPass pass) const override;

  CS_FUNCTION(jenum="cs::eMaterialAttributeType")
  void RegisterAttribute(const std::string & attributeName, cs::eMaterialAttributeType attributeType);
  CS_FUNCTION()
  CS_NODISCARD size_t GetNumberOfAttributes() const;


  CS_FUNCTION()
  CS_NODISCARD std::vector<std::string> GetAttributeNames() const;

  CS_FUNCTION()
  size_t IndexOf(const std::string & attributeName) override;
  void Debug(size_t idx) override;

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

private:
  bool BindShader(iDevice * device, eRenderPass pass) const;
  void BindBlending(iDevice *device) const;
  void BindDepthMode(iDevice *device) const;
  void BindFillMode (iDevice *device) const;
  bool BindAttribute(iDevice * device, eRenderPass pass, size_t idx) const;
  bool BindAttribute(iDevice * device, eRenderPass pass, size_t idx, const std::array<float, 16> &floats, const std::array<int, 4> &ints, cs::iTexture *texture) const;
  static bool BindTexture (iDevice *device, iShaderAttribute *attribute, cs::iTexture *texture);
  void UpdateShaderAttributes(eRenderPass pass);

  struct Attribute
  {
    std::array<iShaderAttribute*, eRP_COUNT> Attributes;
    std::string Name;
    eMaterialAttributeType Type;
    std::array<float, 16> Floats;
    std::array<int, 4> Ints;
    csAssetRef<cs::iTexture>  Texture;
  };


  std::array<csAssetRef<iShader>, eRP_COUNT> m_shader;
  std::vector<Attribute> m_attributes;

  eFillMode m_fillMode = eFillMode::Fill;
  eRenderQueue m_queue = eRenderQueue::Default;

  bool  m_blending = false;
  eBlendFactor m_srcFactorColor = eBlendFactor::One;
  eBlendFactor m_srcFactorAlpha = eBlendFactor::One;
  eBlendFactor m_dstFactorColor = eBlendFactor::Zero;
  eBlendFactor m_dstFactorAlpha = eBlendFactor::Zero;

  bool m_depthWrite = true;
  bool m_depthTest  = true;
  eCompareFunc m_depthFunc = eCF_Less;

  eShadingMode m_shadingMode = eShadingMode::Shaded;
};


}