#pragma once


#include <ceCore/csCoreExport.hh>
#include <ceCore/csTypes.hh>
#include <string>
#include <exception>
#include <map>

namespace cryo
{


class  csBufferExhaustedException : public std::exception
{
public:
  inline csBufferExhaustedException() : std::exception () { }
};

class csBinaryEntryAlreadyExistsException : public std::exception
{
public:
  inline csBinaryEntryAlreadyExistsException() : std::exception() { }
};

class CS_CORE_API csBinaryOutputStream
{
public:
  csBinaryOutputStream();
  ~csBinaryOutputStream();

  csBinaryOutputStream& operator<<(const int8_t d);
  csBinaryOutputStream& operator<<(const uint8_t d);
  csBinaryOutputStream& operator<<(const int16_t d);
  csBinaryOutputStream& operator<<(const uint16_t d);
  csBinaryOutputStream& operator<<(const int32_t d);
  csBinaryOutputStream& operator<<(const uint32_t d);
  csBinaryOutputStream& operator<<(const int64_t d);
  csBinaryOutputStream& operator<<(const uint64_t d);
  csBinaryOutputStream& operator<<(const float d);
  csBinaryOutputStream& operator<<(const double d);
  csBinaryOutputStream& operator<<(const bool d);
  csBinaryOutputStream& operator<<(const std::string& d);
  csBinaryOutputStream& Write(const uint8_t* data, Size size);
  
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

class CS_CORE_API csBinaryInputStream
{
public:
  csBinaryInputStream(const uint8_t* m_buffer, Size bufferSize);
  ~csBinaryInputStream();

  const csBinaryInputStream& operator>>(int8_t& d) const;
  const csBinaryInputStream& operator>>(uint8_t& d) const;
  const csBinaryInputStream& operator>>(int16_t& d) const;
  const csBinaryInputStream& operator>>(uint16_t& d) const;
  const csBinaryInputStream& operator>>(int32_t& d) const;
  const csBinaryInputStream& operator>>(uint32_t& d) const;
  const csBinaryInputStream& operator>>(int64_t& d) const;
  const csBinaryInputStream& operator>>(uint64_t& d) const;
  const csBinaryInputStream& operator>>(float& d) const;
  const csBinaryInputStream& operator>>(double& d) const;
  const csBinaryInputStream& operator>>(bool& d) const;
  const csBinaryInputStream& operator>>(std::string& d) const;
  const csBinaryInputStream& Read(uint8_t* data, Size size) const;

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


class CS_CORE_API csBinaryDictionary
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
  csBinaryDictionary();
  csBinaryDictionary(const csBinaryInputStream &stream);
  ~csBinaryDictionary();

  void Write(csBinaryOutputStream& stream) const;
  void ReadAll(const csBinaryInputStream& stream);
  void ReadHeader(const csBinaryInputStream &stream);
  const Entry Read(const csBinaryInputStream &stream, const std::string &name);

  void Put(const std::string& name, const uint8_t* data, Size dataSize);
  bool Remove(const std::string &name);

  bool Contains(const std::string& name) const;
  const Entry Get(const std::string& name) const;

private:
    void ReadEntry(const csBinaryInputStream &stream, const std::string &name);

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
