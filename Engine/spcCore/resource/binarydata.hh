#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/types.hh>
#include <string>
#include <exception>
#include <map>

namespace spc
{


class  BufferExhaustedException : public std::exception
{
public:
  inline BufferExhaustedException() : std::exception () { }
};

class BinaryEntryAlreadyExistsException : public std::exception
{
public:
  inline BinaryEntryAlreadyExistsException() : std::exception() { }
};

class SPC_CORE_API BinaryOutputStream
{
public:
  BinaryOutputStream();
  ~BinaryOutputStream();

  BinaryOutputStream& operator<<(const Int8 d);
  BinaryOutputStream& operator<<(const UInt8 d);
  BinaryOutputStream& operator<<(const Int16 d);
  BinaryOutputStream& operator<<(const UInt16 d);
  BinaryOutputStream& operator<<(const Int32 d);
  BinaryOutputStream& operator<<(const UInt32 d);
  BinaryOutputStream& operator<<(const Int64 d);
  BinaryOutputStream& operator<<(const UInt64 d);
  BinaryOutputStream& operator<<(const float d);
  BinaryOutputStream& operator<<(const double d);
  BinaryOutputStream& operator<<(const bool d);
  BinaryOutputStream& operator<<(const std::string& d);
  BinaryOutputStream& Write(const UInt8* data, Size size);
  
  Size Tell() const
  {
    return m_idx;
  }

  void Seek(Size newIdx)
  {
    m_idx = newIdx;
  }

  const UInt8* GetData() const
  {
    return m_buffer;
  }

  Size GetSize() const
  {
    return m_idx;
  }


private:

  void Reserve(Size size);

  UInt8* m_buffer;
  Size m_capacity;
  Size m_idx;
  Size m_sizeIncrement;
};

class SPC_CORE_API BinaryInputStream
{
public:
  BinaryInputStream(const UInt8* m_buffer, Size bufferSize);
  ~BinaryInputStream();

  const BinaryInputStream& operator>>(Int8& d) const;
  const BinaryInputStream& operator>>(UInt8& d) const;
  const BinaryInputStream& operator>>(Int16& d) const;
  const BinaryInputStream& operator>>(UInt16& d) const;
  const BinaryInputStream& operator>>(Int32& d) const;
  const BinaryInputStream& operator>>(UInt32& d) const;
  const BinaryInputStream& operator>>(Int64& d) const;
  const BinaryInputStream& operator>>(UInt64& d) const;
  const BinaryInputStream& operator>>(float& d) const;
  const BinaryInputStream& operator>>(double& d) const;
  const BinaryInputStream& operator>>(bool& d) const;
  const BinaryInputStream& operator>>(std::string& d) const;
  const BinaryInputStream& Read(UInt8* data, Size size) const;

  Size Tell() const
  {
    return m_idx;
  }

  void Seek(Size newIdx) const
  {
    m_idx = newIdx;
  }
private:
  void CheckSize(Size size) const;

  const UInt8* m_buffer;
  Size m_bufferSize;
  mutable Size m_idx;
};


class SPC_CORE_API BinaryDictionary
{
public:
  struct Entry
  {
    const std::string Name;
    const UInt8* Data;
    const Size DataSize;
    Entry(const std::string Name, const UInt8* Data, const Size DataSize)
      : Name(Name)
      , Data(Data)
      , DataSize(DataSize)
    {

    }
  };

public:
  BinaryDictionary();
  BinaryDictionary(const BinaryInputStream &stream);
  ~BinaryDictionary();

  void Write(BinaryOutputStream& stream) const;
  void ReadAll(const BinaryInputStream& stream);
  void ReadHeader(const BinaryInputStream &stream);
  const Entry Read(const BinaryInputStream &stream, const std::string &name);

  void Put(const std::string& name, const UInt8* data, Size dataSize);
  bool Remove(const std::string &name);

  bool Contains(const std::string& name) const;
  const Entry Get(const std::string& name) const;

private:
    void ReadEntry(const BinaryInputStream &stream, const std::string &name);

  struct _Entry
  {
    std::string Name;
    UInt8* m_data;
    Size m_dataSize;
  };

  std::map<std::string, _Entry> m_entries;
  std::map<std::string, Size> m_header;

};


}
