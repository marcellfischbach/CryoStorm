#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/resource/eaccessmode.hh>
#include <ceCore/resource/eopenmode.hh>
#include <ceCore/resource/eseekmode.hh>

namespace ce
{



CE_CLASS()
struct CE_CORE_API iFile : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;
  virtual ~iFile() {}

  virtual bool Open(eAccessMode accessMode, eOpenMode openMode = eOM_Binary ) = 0;
  virtual bool IsOpen() const = 0;
  virtual bool IsEOF() const = 0;
  virtual void Seek(eSeekMode mode, long offset) = 0;
  virtual long Tell() const = 0;
  virtual Size Read(Size elementSize, Size elementCount, void* buffer) = 0;
  virtual Size Write(Size elementSize, Size elementCount, const void* buffer) = 0;
  virtual void Close() = 0;

};
}