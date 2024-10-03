
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/graphics/ebufferusage.hh>


namespace ce
{

CS_CLASS()
struct CS_CORE_API iIndexBuffer : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  virtual void CreateForRendering(Size size, eBufferUsage usage = eBU_Static) = 0;

  virtual void Bind() = 0;

  virtual void Unbind() = 0;


  virtual void Map(void** data, Size &dataSize) = 0;

  virtual void Unmap() = 0;

  virtual void Copy(const void* data, Size count, Size targetOffset = 0) = 0;


};


}
