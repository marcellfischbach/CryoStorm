#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/resource/eaccessmode.hh>
#include <spcCore/resource/eopenmode.hh>
#include <spcCore/resource/eseekmode.hh>

namespace spc
{



SPC_CLASS()
struct SPC_CORE_API iFile : public SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
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