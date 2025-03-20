//
// Created by Marcell on 11.05.2024.
//

#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/resource/iFile.hh>


namespace cs
{



CS_CLASS(jclass="org.cryo.core.resource.JavaFile")
class csJavaFile : public CS_SUPER(iFile)
{
  CS_CLASS_GEN_OBJECT;
public:
  csJavaFile();
  ~csJavaFile() override;

  const std::string &GetName() const override;
  bool Open(eAccessMode accessMode, eOpenMode openMode) override;
  bool IsOpen() const override;
  bool IsEOF() const override;
  void Seek(eSeekMode mode, long offset) override;
  long Tell() const override;
  Size Read(Size elementSize, Size elementCount, void *buffer) override;
  Size Write(Size elementSize, Size elementCount, const void *buffer) override;
  void Close() override;

private:
  mutable std::string m_name;
  struct csJavaFilePrivate *m_priv;
};


}