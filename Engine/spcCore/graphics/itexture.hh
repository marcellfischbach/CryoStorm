

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/etexturetype.hh>

namespace spc
{

SPC_CLASS(NoInstance)
struct SPC_CORE_API iTexture : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  virtual ~iTexture() { }

  virtual eTextureType GetType() const = 0;

};


}