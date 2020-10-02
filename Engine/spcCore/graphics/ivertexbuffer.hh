
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/graphics/bufferusage.hh>


namespace spc
{

SPC_CLASS()
struct SPC_CORE_API iVertexBuffer : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  virtual void CreateForRendering(Size size, eBufferUsage usage = eBU_Static) = 0;

  virtual void Bind() = 0;

  virtual void Unbind() = 0;

  virtual void Copy(const void* data, Size count, Size targetOffset = 0) = 0;


};


}
