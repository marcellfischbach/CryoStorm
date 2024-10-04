#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csTypes.hh>
#include <csCore/resource/eAccessMode.hh>
#include <csCore/resource/eOpenMode.hh>
#include <csCore/resource/eSeekMode.hh>

namespace cryo
{



CS_CLASS()
struct CS_CORE_API iFile : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  virtual ~iFile() {}

  virtual const std::string &GetName() const = 0;
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