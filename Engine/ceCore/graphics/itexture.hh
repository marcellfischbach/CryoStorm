

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/etexturetype.hh>
#include <ceCore/graphics/epixelformat.hh>

namespace ce
{

struct iSampler;

CE_CLASS(virtual)
struct CE_CORE_API iTexture : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;
  ~iTexture() override { }

  virtual eTextureType GetType() const = 0;

  virtual ePixelFormat GetFormat() const = 0;

  virtual void SetSampler(iSampler *sampler) = 0;
  virtual iSampler* GetSampler() = 0;
  virtual const iSampler* GetSampler() const = 0;

};


}