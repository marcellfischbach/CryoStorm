

#include <spcCore/resource/binarydata.hh>

namespace spc
{

BufferExhaustedException::BufferExhaustedException()
  : std::exception()
{

}



/* ************************************************************************
 * ************************************************************************
 * 	Binary Output Stream
 * ************************************************************************
 * ************************************************************************
 */

BinaryOutputStream::BinaryOutputStream()
  : m_buffer(nullptr), m_idx(0), m_capacity(0), m_sizeIncrement(256)
{

}

BinaryOutputStream::~BinaryOutputStream()
{
  if (m_buffer)
  {
    delete[] m_buffer;
    m_buffer = nullptr;
  }

  m_capacity = 0;
  m_idx = 0;
}


BinaryOutputStream& BinaryOutputStream::operator<<(const Int8 d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const UInt8 d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const Int16 d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const UInt16 d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const Int32 d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const UInt32 d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const Int64 d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const UInt64 d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const float d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const double d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const bool d)
{
  Reserve(sizeof(d));
  memcpy(m_buffer + m_idx, &d, sizeof(d));
  m_idx += sizeof(d);
  return *this;
}

BinaryOutputStream& BinaryOutputStream::operator<<(const std::string& d)
{
  Size size = d.size();
  *this << size;
  Reserve(sizeof(char) * d.size());
  memcpy(m_buffer + m_idx, d.c_str(), sizeof(char) * d.size());
  m_idx += sizeof(char) * d.size();
  return *this;
}

BinaryOutputStream& BinaryOutputStream::Write(const UInt8* data, Size size)
{
  Reserve(size);
  memcpy(m_buffer + m_idx, data, size);
  m_idx += size;
}


void BinaryOutputStream::Reserve(Size size)
{
  Size remaining = m_capacity - m_idx;
  if (remaining >= size)
  {
    return;
  }
  Size missing = size - remaining;
  Size parts = missing / m_sizeIncrement;
  Size rest = missing % m_sizeIncrement;
  if (rest != 0)
  {
    parts++;
  }

  Size newCapacity = m_capacity + parts + m_sizeIncrement;
  UInt8* newBuffer = new UInt8[newCapacity];
  memcpy(newBuffer, m_buffer, m_capacity);
  delete[] m_buffer;

  m_buffer = newBuffer;
  m_capacity = newCapacity;

}


/* ************************************************************************
 * ************************************************************************
 * 	Binary Input Stream
 * ************************************************************************
 * ************************************************************************
 */


BinaryInputStream::BinaryInputStream(const UInt8* buffer, Size bufferSize)
  : m_buffer(buffer), m_idx(0), m_bufferSize(bufferSize)
{

}

BinaryInputStream::~BinaryInputStream()
{
  m_buffer = nullptr;
  m_bufferSize = 0;
  m_idx = 0;
}


const BinaryInputStream& BinaryInputStream::operator>>(Int8 &d) const
{
  CheckSize(sizeof(Int8));
  memcpy(&d, m_buffer + m_idx, sizeof(Int8));
  m_idx += sizeof(Int8);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(UInt8 &d) const
{
  CheckSize(sizeof(UInt8));
  memcpy(&d, m_buffer + m_idx, sizeof(UInt8));
  m_idx += sizeof(UInt8);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(Int16 &d) const
{
  CheckSize(sizeof(Int16));
  memcpy(&d, m_buffer + m_idx, sizeof(Int16));
  m_idx += sizeof(Int16);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(UInt16 &d) const
{
  CheckSize(sizeof(UInt16));
  memcpy(&d, m_buffer + m_idx, sizeof(UInt16));
  m_idx += sizeof(UInt16);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(Int32 &d) const
{
  CheckSize(sizeof(Int32));
  memcpy(&d, m_buffer + m_idx, sizeof(Int32));
  m_idx += sizeof(Int32);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(UInt32 &d) const
{
  CheckSize(sizeof(UInt32));
  memcpy(&d, m_buffer + m_idx, sizeof(UInt32));
  m_idx += sizeof(UInt32);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(Int64 &d) const
{
  CheckSize(sizeof(Int64));
  memcpy(&d, m_buffer + m_idx, sizeof(Int64));
  m_idx += sizeof(Int64);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(UInt64 &d) const
{
  CheckSize(sizeof(UInt64));
  memcpy(&d, m_buffer + m_idx, sizeof(UInt64));
  m_idx += sizeof(UInt64);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(float &d) const
{
  CheckSize(sizeof(float));
  memcpy(&d, m_buffer + m_idx,  sizeof(float));
  m_idx += sizeof(float);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(double &d) const
{
  CheckSize(sizeof(double));
  memcpy(&d, m_buffer + m_idx, sizeof(double));
  m_idx += sizeof(double);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(bool &d)  const
{
  CheckSize(sizeof(bool));
  memcpy(&d, m_buffer + m_idx, sizeof(bool));
  m_idx += sizeof(bool);
  return *this;
}

const BinaryInputStream& BinaryInputStream::operator>>(std::string &d) const
{
  Size size;
  *this >> size;
  CheckSize(sizeof(char) * d.size());
  d = std::string(reinterpret_cast<const char*>(m_buffer + m_idx), size);
  m_idx += sizeof(char) * d.size();
  return *this;
}

const BinaryInputStream& BinaryInputStream::Read(UInt8* buffer, Size size) const
{
  CheckSize(size);
  memcpy(buffer, m_buffer + m_idx, sizeof(size));
  m_idx += size;
  return *this;
}

void BinaryInputStream::CheckSize(Size size) const
{
  if (m_idx + size > m_bufferSize)
  {
    throw BufferExhaustedException();
  }
}



BinaryDictionary::BinaryDictionary()
{

}

void BinaryDictionary::Write(BinaryOutputStream& stream) const
{
  stream << m_entries.size();
  for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
  {
    stream << it->first
      << it->second.m_dataSize;
    stream.Write(it->second.m_data, it->second.m_dataSize);
  }
}

void BinaryDictionary::Read(const BinaryInputStream& stream)
{
  m_entries.clear();
  Size size;
  stream >> size;
  for (Size i = 0; i < size; i++)
  {
    std::string name;
    Size dataSize;
    stream >> name
      >> dataSize;

    _Entry entry;
    entry.Name = name;
    entry.m_data = new UInt8[dataSize];
    entry.m_dataSize = dataSize;
    stream.Read(entry.m_data, dataSize);
    m_entries[name] = entry;
  }
}

void BinaryDictionary::Put(const std::string& name, const UInt8* data, Size dataSize)
{
  if (Contains(name))
  {
    return;
  }

}

bool BinaryDictionary::Contains(const std::string& name) const
{
  return m_entries.find(name) != m_entries.end();
}

const BinaryDictionary::Entry BinaryDictionary::Get(const std::string& name) const
{
  auto it = m_entries.find(name);
  if (it != m_entries.end())
  { 
    return Entry(name, it->second.m_data, it->second.m_dataSize);
  }
  return Entry(name, nullptr, 0);
}


}