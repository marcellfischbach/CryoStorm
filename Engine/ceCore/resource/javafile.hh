//
// Created by Marcell on 11.05.2024.
//

#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/resource/ifile.hh>


namespace ce
{



CE_CLASS()
class JavaFile : public CE_SUPER(iFile)
{
public:
  const std::string &GetName() const override;
  bool Open(eAccessMode accessMode, eOpenMode openMode) override;
  bool IsOpen() const override;
  bool IsEOF() const override;
  void Seek(eSeekMode mode, long offset) override;
  long Tell() const override;
  Size Read(Size elementSize, Size elementCount, void *buffer) override;
  Size Write(Size elementSize, Size elementCount, const void *buffer) override;
  void Close() override;
};


}