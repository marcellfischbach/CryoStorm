

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
public:
  static const Size UndefinedIndex = ~0x00;

  csMaterial();
  ~csMaterial() override;

  bool Bind(iDevice * device, eRenderPass pass) override;

  void SetFillMode (eFillMode fillMode);
  CS_NODISCARD eFillMode GetFillMode () const override;

  void SetRenderQueue(eRenderQueue queue);
  CS_NODISCARD eRenderQueue GetRenderQueue() const override;

  void SetBlending(bool  blending);
  CS_NODISCARD bool IsBlending () const;
  void SetBlendFactor (eBlendFactor srcFactor, eBlendFactor dstFactor);
  void SetBlendFactor (eBlendFactor srcFactorColor, eBlendFactor srcFactorAlpha, eBlendFactor dstFactorColor, eBlendFactor dstFactorAlpha);
  CS_NODISCARD eBlendFactor GetBlendFactorSrcColor () const;
  CS_NODISCARD eBlendFactor GetBlendFactorSrcAlpha () const;
  CS_NODISCARD eBlendFactor GetBlendFactorDstColor () const;
  CS_NODISCARD eBlendFactor GetBlendFactorDstAlpha () const;

  void SetDepthWrite (bool depthWrite);
  CS_NODISCARD bool IsDepthWrite() const;
  void SetDepthTest (bool depthTest);
  CS_NODISCARD bool IsDepthTest() const;
  void SetDepthFunc (eCompareFunc depthFun);
  CS_NODISCARD eCompareFunc GetDepthFunc() const;
  void SetShadingMode (eShadingMode shadingMode);
  CS_NODISCARD eShadingMode GetShadingMode () const override;


  void SetShader(eRenderPass pass, iShader * shader);
  CS_NODISCARD iShader* GetShader(eRenderPass pass);
  CS_NODISCARD const iShader* GetShader(eRenderPass pass) const override;

  void RegisterAttribute(const std::string & attributeName, eMaterialAttributeType attributeType);
  CS_NODISCARD uint16_t GetNumberOfAttributes() const;

  CS_NODISCARD std::vector<std::string> GetAttributeNames() const;

  Size IndexOf(const std::string & attributeName) override;
  void Debug(Size idx) override;

  void Set(Size idx, float value) override;
  void Set(Size idx, const csVector2f & v) override;
  void Set(Size idx, const csVector3f & v) override;
  void Set(Size idx, const csVector4f & v) override;
  void Set(Size idx, const csColor4f & v) override;
  void Set(Size idx, int value) override;
  void Set(Size idx, const csMatrix3f & m) override;
  void Set(Size idx, const csMatrix4f & m) override;
  void Set(Size idx, iTexture * texture) override;


private:
  bool BindShader(iDevice * device, eRenderPass pass) const;
  void BindBlending(iDevice *device) const;
  void BindDepthMode(iDevice *device) const;
  void BindFillMode (iDevice *device) const;
  bool BindAttribute(iDevice * device, eRenderPass pass, size_t idx) const;
  bool BindAttribute(iDevice * device, eRenderPass pass, size_t idx, const std::array<float, 16> &floats, const std::array<int, 4> &ints, iTexture * texture) const;
  static bool BindTexture (iDevice *device, iShaderAttribute *attribute, iTexture *texture);
  void UpdateShaderAttributes(eRenderPass pass);

  struct Attribute
  {
    std::array<iShaderAttribute*, eRP_COUNT> Attributes;
    std::string Name;
    eMaterialAttributeType Type;
    std::array<float, 16> Floats;
    std::array<int, 4> Ints;
    iTexture* Texture;
  };


  std::array<iShader*, eRP_COUNT> m_shader;
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