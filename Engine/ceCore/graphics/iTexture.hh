

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/eTextureType.hh>
#include <ceCore/graphics/ePixelFormat.hh>

namespace cryo
{

struct iSampler;

CS_CLASS(virtual, jclass="org.crimsonedge.core.graphics.ITexture")
struct CS_CORE_API iTexture : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iTexture() override { }

  virtual eTextureType GetType() const = 0;

  virtual ePixelFormat GetFormat() const = 0;

  virtual void SetSampler(iSampler *sampler) = 0;
  virtual iSampler* GetSampler() = 0;
  virtual const iSampler* GetSampler() const = 0;

};


}