
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/graphics/ecomparefunc.hh>
#include <ceCore/graphics/efiltermode.hh>
#include <ceCore/graphics/etextureaddressmode.hh>
#include <ceCore/graphics/etexturecomparemode.hh>
#include <ceCore/math/color4f.hh>

namespace ce
{

CE_CLASS()
struct CE_CORE_API iSampler : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;
  virtual ~iSampler() {}

  virtual void SetFilterMode(eFilterMode filterMode) = 0;
  virtual eFilterMode GetFilterMode() const = 0;

  virtual void SetAnisotropy(uint8_t anisotropy) = 0;
  virtual uint8_t GetAnisotropy() const = 0;

  virtual void SetMinLOD(int16_t minLOD) = 0;
  virtual int16_t GetMinLOD() const = 0;

  virtual void SetMaxLOD(int16_t maxLOD) = 0;
  virtual int16_t GetMaxLOD() const = 0;

  virtual void SetAddressU(eTextureAddressMode mode) = 0;
  virtual eTextureAddressMode GetAddressU() const = 0;

  virtual void SetAddressV(eTextureAddressMode mode) = 0;
  virtual eTextureAddressMode GetAddressV() const = 0;

  virtual void SetAddressW(eTextureAddressMode mode) = 0;
  virtual eTextureAddressMode GetAddressW() const = 0;

  virtual void SetBorderColor(const Color4f& borderColo) = 0;
  virtual const Color4f& GetBorderColor() const = 0;


  virtual void SetTextureCompareMode(eTextureCompareMode compareMode) = 0;
  virtual eTextureCompareMode GetTextureCompareMode() const = 0;

  virtual void SetTextureCompareFunc(eCompareFunc compareMode) = 0;
  virtual eCompareFunc GetTextureCompareFunc() const = 0;

  virtual bool NeedsMipMaps() const = 0;

};


}