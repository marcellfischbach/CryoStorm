

#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/eblendfactor.hh>
#include <spcCore/graphics/edepthmode.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <spcCore/graphics/eshadingmode.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/itexture.hh>
#include <spcCore/graphics/material/ematerialattributetype.hh>
#include <spcCore/graphics/material/imaterial.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/math/matrix.hh>
#include <spcCore/math/vector.hh>
#include <string>
#include <array>

namespace spc
{

struct iShader;
struct iShaderAttribute;
struct iTexture;


SPC_CLASS()
class SPC_CORE_API Material : public SPC_SUPER(iMaterial)
{
  friend class MaterialInstance;
  SPC_CLASS_GEN_OBJECT;
public:
  static const Size UndefinedIndex = ~0x00;

  Material();
  ~Material() override;

  bool Bind(iDevice * device, eRenderPass pass) override;


  void SetRenderQueue(eRenderQueue queue);
  SPC_NODISCARD eRenderQueue GetRenderQueue() const override;

  void SetBlending(bool  blending);
  SPC_NODISCARD bool IsBlending () const;
  void SetBlendFactor (eBlendFactor srcFactor, eBlendFactor dstFactor);
  void SetBlendFactor (eBlendFactor srcFactorColor, eBlendFactor srcFactorAlpha, eBlendFactor dstFactorColor, eBlendFactor dstFactorAlpha);
  SPC_NODISCARD eBlendFactor GetBlendFactorSrcColor () const;
  SPC_NODISCARD eBlendFactor GetBlendFactorSrcAlpha () const;
  SPC_NODISCARD eBlendFactor GetBlendFactorDstColor () const;
  SPC_NODISCARD eBlendFactor GetBlendFactorDstAlpha () const;

  void SetDepthWrite (bool depthWrite);
  SPC_NODISCARD bool IsDepthWrite() const;
  void SetDepthTest (bool depthTest);
  SPC_NODISCARD bool IsDepthTest() const;
  void SetShadingMode (eShadingMode shadingMode);
  SPC_NODISCARD eShadingMode GetShadingMode () const override;


  void SetShader(eRenderPass pass, iShader * shader);
  SPC_NODISCARD iShader* GetShader(eRenderPass pass);
  SPC_NODISCARD const iShader* GetShader(eRenderPass pass) const override;

  void RegisterAttribute(const std::string & attributeName, eMaterialAttributeType attributeType);
  SPC_NODISCARD uint16_t GetNumberOfAttributes() const;

  SPC_NODISCARD std::vector<std::string> GetAttributeNames() const;

  Size IndexOf(const std::string & attributeName) override;

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

  eRenderQueue m_queue = eRenderQueue::Default;

  bool  m_blending = false;
  eBlendFactor m_srcFactorColor = eBlendFactor::One;
  eBlendFactor m_srcFactorAlpha = eBlendFactor::One;
  eBlendFactor m_dstFactorColor = eBlendFactor::Zero;
  eBlendFactor m_dstFactorAlpha = eBlendFactor::Zero;

  bool m_depthWrite = true;
  bool m_depthTest  = true;

  eShadingMode m_shadingMode = eShadingMode::Shaded;
};


}