#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/ifile.hh>
#include <string>
#include <stdio.h>

namespace ce
{

CE_CLASS()
class CE_CORE_API FileSystemFile : public CE_SUPER(iFile)
{
  CE_CLASS_GEN_OBJECT;
public:
  FileSystemFile(const std::string & path);

  const std::string &GetName() const override;
  bool Open(eAccessMode accessMode, eOpenMode openMode = eOM_Binary) override;
  bool IsOpen() const override;
  bool IsEOF() const override;
  void Seek(eSeekMode mode, long offset) override;
  long Tell() const override;
  Size Read(Size elementSize, Size elementCount, void* buffer) override;
  Size Write(Size elementSize, Size elementCount, const void* buffer) override;
  void Close() override;

  FILE* GetHandle() const;

private:
  std::string m_path;
  FILE* m_file;
};

}
