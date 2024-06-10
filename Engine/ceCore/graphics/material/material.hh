

#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/graphics/eblendfactor.hh>
#include <ceCore/graphics/edepthmode.hh>
#include <ceCore/graphics/erenderpass.hh>
#include <ceCore/graphics/eshadingmode.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/itexture.hh>
#include <ceCore/graphics/material/ematerialattributetype.hh>
#include <ceCore/graphics/material/imaterial.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/math/matrix.hh>
#include <ceCore/math/vector.hh>
#include <string>
#include <array>

namespace ce
{

struct iShader;
struct iShaderAttribute;
struct iTexture;


CE_CLASS(jclass="org.crimsonedge.core.graphics.material.Material")
class CE_CORE_API Material : public CE_SUPER(iMaterial)
{
  friend class MaterialInstance;
  CE_CLASS_GEN_OBJECT;
public:
  static const Size UndefinedIndex = ~0x00;

  Material();
  ~Material() override;

  bool Bind(iDevice * device, eRenderPass pass) override;

  void SetFillMode (eFillMode fillMode);
  CE_NODISCARD eFillMode GetFillMode () const override;

  void SetRenderQueue(eRenderQueue queue);
  CE_NODISCARD eRenderQueue GetRenderQueue() const override;

  void SetBlending(bool  blending);
  CE_NODISCARD bool IsBlending () const;
  void SetBlendFactor (eBlendFactor srcFactor, eBlendFactor dstFactor);
  void SetBlendFactor (eBlendFactor srcFactorColor, eBlendFactor srcFactorAlpha, eBlendFactor dstFactorColor, eBlendFactor dstFactorAlpha);
  CE_NODISCARD eBlendFactor GetBlendFactorSrcColor () const;
  CE_NODISCARD eBlendFactor GetBlendFactorSrcAlpha () const;
  CE_NODISCARD eBlendFactor GetBlendFactorDstColor () const;
  CE_NODISCARD eBlendFactor GetBlendFactorDstAlpha () const;

  void SetDepthWrite (bool depthWrite);
  CE_NODISCARD bool IsDepthWrite() const;
  void SetDepthTest (bool depthTest);
  CE_NODISCARD bool IsDepthTest() const;
  void SetDepthFunc (eCompareFunc depthFun);
  CE_NODISCARD eCompareFunc GetDepthFunc() const;
  void SetShadingMode (eShadingMode shadingMode);
  CE_NODISCARD eShadingMode GetShadingMode () const override;


  void SetShader(eRenderPass pass, iShader * shader);
  CE_NODISCARD iShader* GetShader(eRenderPass pass);
  CE_NODISCARD const iShader* GetShader(eRenderPass pass) const override;

  void RegisterAttribute(const std::string & attributeName, eMaterialAttributeType attributeType);
  CE_NODISCARD uint16_t GetNumberOfAttributes() const;

  CE_NODISCARD std::vector<std::string> GetAttributeNames() const;

  Size IndexOf(const std::string & attributeName) override;
  void Debug(Size idx) override;

  void Set(Size idx, float value) override;
  void Set(Size idx, const Vector2f & v) override;
  void Set(Size idx, const Vector3f & v) override;
  void Set(Size idx, const Vector4f & v) override;
  void Set(Size idx, const Color4f & v) override;
  void Set(Size idx, int value) override;
  void Set(Size idx, const Matrix3f & m) override;
  void Set(Size idx, const Matrix4f & m) override;
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