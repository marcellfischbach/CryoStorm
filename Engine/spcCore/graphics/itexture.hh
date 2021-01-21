

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/etexturetype.hh>

namespace spc
{

struct iSampler;

SPC_CLASS(NoInstance)
struct SPC_CORE_API iTexture : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  virtual ~iTexture() { }

  virtual eTextureType GetType() const = 0;

  virtual void SetSampler(iSampler *sampler) = 0;
  virtual iSampler* GetSampler() = 0;
  virtual const iSampler* GetSampler() const = 0;

};


}