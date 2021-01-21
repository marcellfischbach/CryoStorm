
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/ecomparefunc.hh>
#include <spcCore/graphics/efiltermode.hh>
#include <spcCore/graphics/etextureaddressmode.hh>
#include <spcCore/graphics/etexturecomparemode.hh>
#include <spcCore/math/color4f.hh>

namespace spc
{

SPC_CLASS()
struct SPC_CORE_API iSampler : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  virtual ~iSampler() {}

  virtual void SetFilterMode(eFilterMode filterMode) = 0;
  virtual eFilterMode GetFilterMode() const = 0;

  virtual void SetAnisotropy(UInt8 anisotropy) = 0;
  virtual UInt8 GetAnisotropy() const = 0;

  virtual void SetMinLOD(UInt16 minLOD) = 0;
  virtual UInt16 GetMinLOD() const = 0;

  virtual void SetMaxLOD(UInt16 maxLOD) = 0;
  virtual UInt16 GetMaxLOD() const = 0;

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