#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/types.hh>
#include <string>
#include <exception>
#include <map>

namespace ce
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

class CE_CORE_API BinaryOutputStream
{
public:
  BinaryOutputStream();
  ~BinaryOutputStream();

  BinaryOutputStream& operator<<(const Int8 d);
  BinaryOutputStream& operator<<(const uint8_t d);
  BinaryOutputStream& operator<<(const Int16 d);
  BinaryOutputStream& operator<<(const uint16_t d);
  BinaryOutputStream& operator<<(const Int32 d);
  BinaryOutputStream& operator<<(const uint32_t d);
  BinaryOutputStream& operator<<(const Int64 d);
  BinaryOutputStream& operator<<(const uint64_t d);
  BinaryOutputStream& operator<<(const float d);
  BinaryOutputStream& operator<<(const double d);
  BinaryOutputStream& operator<<(const bool d);
  BinaryOutputStream& operator<<(const std::string& d);
  BinaryOutputStream& Write(const uint8_t* data, Size size);
  
  Size Tell() const
  {
    return m_idx;
  }

  void Seek(Size newIdx)
  {
    m_idx = newIdx;
  }

  const uint8_t* GetData() const
  {
    return m_buffer;
  }

  Size GetSize() const
  {
    return m_idx;
  }


private:

  void Reserve(Size size);

  uint8_t* m_buffer;
  Size m_capacity;
  Size m_idx;
  Size m_sizeIncrement;
};

class CE_CORE_API BinaryInputStream
{
public:
  BinaryInputStream(const uint8_t* m_buffer, Size bufferSize);
  ~BinaryInputStream();

  const BinaryInputStream& operator>>(Int8& d) const;
  const BinaryInputStream& operator>>(uint8_t& d) const;
  const BinaryInputStream& operator>>(Int16& d) const;
  const BinaryInputStream& operator>>(uint16_t& d) const;
  const BinaryInputStream& operator>>(Int32& d) const;
  const BinaryInputStream& operator>>(uint32_t& d) const;
  const BinaryInputStream& operator>>(Int64& d) const;
  const BinaryInputStream& operator>>(uint64_t& d) const;
  const BinaryInputStream& operator>>(float& d) const;
  const BinaryInputStream& operator>>(double& d) const;
  const BinaryInputStream& operator>>(bool& d) const;
  const BinaryInputStream& operator>>(std::string& d) const;
  const BinaryInputStream& Read(uint8_t* data, Size size) const;

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

  const uint8_t* m_buffer;
  Size m_bufferSize;
  mutable Size m_idx;
};


class CE_CORE_API BinaryDictionary
{
public:
  struct Entry
  {
    const std::string Name;
    const uint8_t* Data;
    const Size DataSize;
    Entry(const std::string Name, const uint8_t* Data, const Size DataSize)
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

  void Put(const std::string& name, const uint8_t* data, Size dataSize);
  bool Remove(const std::string &name);

  bool Contains(const std::string& name) const;
  const Entry Get(const std::string& name) const;

private:
    void ReadEntry(const BinaryInputStream &stream, const std::string &name);

  struct _Entry
  {
    std::string Name;
    uint8_t* m_data;
    Size m_dataSize;
  };

  std::map<std::string, _Entry> m_entries;
  std::map<std::string, Size> m_header;

};


}
