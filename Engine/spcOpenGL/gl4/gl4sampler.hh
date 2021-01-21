
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/etextureunit.hh>
#include <spcCore/graphics/isampler.hh>


namespace spc::opengl
{


SPC_CLASS()
class SPC_OGL_API GL4Sampler : public SPC_SUPER(iSampler)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4Sampler();
  virtual ~GL4Sampler();


  void Bind(eTextureUnit unit);
  static void Unbind(eTextureUnit unit);

  void SetFilterMode(eFilterMode filterMode) override;
  eFilterMode GetFilterMode() const override;

  void SetAnisotropy(UInt8 anisotropy) override;
  UInt8 GetAnisotropy() const override;

  void SetMinLOD(UInt16 minLOD) override;
  UInt16 GetMinLOD() const override;

  void SetMaxLOD(UInt16 maxLOD) override;
  UInt16 GetMaxLOD() const override;

  void SetAddressU(eTextureAddressMode mode) override;
  eTextureAddressMode GetAddressU() const override;

  void SetAddressV(eTextureAddressMode mode) override;
  eTextureAddressMode GetAddressV() const override;

  void SetAddressW(eTextureAddressMode mode) override;
  eTextureAddressMode GetAddressW() const override;

  void SetBorderColor(const Color4f & borderColo) override;
  const Color4f& GetBorderColor() const override;

  void SetTextureCompareMode(eTextureCompareMode compareMode) override;
  eTextureCompareMode GetTextureCompareMode() const override;

  void SetTextureCompareFunc(eCompareFunc compareMode) override;
  eCompareFunc GetTextureCompareFunc() const override;

  bool NeedsMipMaps() const override;

private:
  UInt32 m_name;

  eFilterMode m_filterMode;
  UInt8 m_anisotropy;

  UInt16 m_minLOD;
  UInt16 m_maxLOD;

  eTextureAddressMode m_addressU;
  eTextureAddressMode m_addressV;
  eTextureAddressMode m_addressW;

  Color4f m_borderColor;

  eTextureCompareMode m_textureCompareMode;
  eCompareFunc m_textureCompareFunc;
};


}