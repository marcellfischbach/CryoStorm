
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/eTextureUnit.hh>
#include <ceCore/graphics/iSampler.hh>


namespace cryo::opengl
{


CS_CLASS()
class CS_OGL_API GL4Sampler : public CS_SUPER(iSampler)
{
  CS_CLASS_GEN_OBJECT;
public:
  GL4Sampler();
  virtual ~GL4Sampler();


  void Bind(eTextureUnit unit);
  static void Unbind(eTextureUnit unit);

  void SetFilterMode(eFilterMode filterMode) override;
  eFilterMode GetFilterMode() const override;

  void SetAnisotropy(uint8_t anisotropy) override;
  uint8_t GetAnisotropy() const override;

  void SetMinLOD(int16_t minLOD) override;
  int16_t GetMinLOD() const override;

  void SetMaxLOD(int16_t maxLOD) override;
  int16_t GetMaxLOD() const override;

  void SetAddressU(eTextureAddressMode mode) override;
  eTextureAddressMode GetAddressU() const override;

  void SetAddressV(eTextureAddressMode mode) override;
  eTextureAddressMode GetAddressV() const override;

  void SetAddressW(eTextureAddressMode mode) override;
  eTextureAddressMode GetAddressW() const override;

  void SetBorderColor(const csColor4f & borderColo) override;
  const csColor4f& GetBorderColor() const override;

  void SetTextureCompareMode(eTextureCompareMode compareMode) override;
  eTextureCompareMode GetTextureCompareMode() const override;

  void SetTextureCompareFunc(eCompareFunc compareMode) override;
  eCompareFunc GetTextureCompareFunc() const override;

  bool NeedsMipMaps() const override;

private:
  uint32_t m_name;

  eFilterMode m_filterMode;
  uint8_t m_anisotropy;

  int16_t m_minLOD;
  int16_t m_maxLOD;

  eTextureAddressMode m_addressU;
  eTextureAddressMode m_addressV;
  eTextureAddressMode m_addressW;

  csColor4f m_borderColor;

  eTextureCompareMode m_textureCompareMode;
  eCompareFunc m_textureCompareFunc;
};


}