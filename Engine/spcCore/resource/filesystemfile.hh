#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/resource/ifile.hh>
#include <string>
#include <stdio.h>

namespace spc
{

SPC_CLASS()
class SPC_CORE_API FileSystemFile : public SPC_SUPER(iFile)
{
  SPC_CLASS_GEN_OBJECT;
public:
  FileSystemFile(const std::string & path);

  bool Open(eAccessMode accessMode, eOpenMode openMode = eOM_Binary) override;
  bool IsOpen() const override;
  bool IsEOF() const override;
  void Seek(eSeekMode mode, long offset) override;
  long Tell() const override;
  Size Read(Size elementSize, Size elementCount, void* buffer) override;
  Size Write(Size elementSize, Size elementCount, const void* buffer) override;
  void Close() override;

private:
  std::string m_path;
  FILE* m_file;
};

}
