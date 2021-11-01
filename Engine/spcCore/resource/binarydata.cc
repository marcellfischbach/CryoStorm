

#include <spcCore/resource/binarydata.hh>

namespace spc
{
    
 
/* ************************************************************************
 * ************************************************************************
 * 	Binary Output Stream
 * ************************************************************************
 * ************************************************************************
 */
    
    BinaryOutputStream::BinaryOutputStream()
      : m_buffer(nullptr)
      , m_capacity(0)
      , m_idx(0)
      , m_sizeIncrement(256)
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
    
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const Int8 d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const uint8_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const Int16 d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const uint16_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const Int32 d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const uint32_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const Int64 d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const uint64_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const float d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const double d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const bool d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::operator<<(const std::string &d)
    {
      Size size = d.size();
      *this << size;
      Reserve(sizeof(char) * d.size());
      memcpy(m_buffer + m_idx, d.c_str(), sizeof(char) * d.size());
      m_idx += sizeof(char) * d.size();
      return *this;
    }
    
    BinaryOutputStream &BinaryOutputStream::Write(const uint8_t *data, Size size)
    {
      Reserve(size);
      memcpy(m_buffer + m_idx, data, size);
      m_idx += size;
      return *this;
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
      uint8_t *newBuffer = new uint8_t[newCapacity];
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
    
    
    BinaryInputStream::BinaryInputStream(const uint8_t *buffer, Size bufferSize)
      : m_buffer(buffer)
      , m_bufferSize(bufferSize)
      , m_idx(0)
    {
    
    }
    
    BinaryInputStream::~BinaryInputStream()
    {
      m_buffer = nullptr;
      m_bufferSize = 0;
      m_idx = 0;
    }
    
    
    const BinaryInputStream &BinaryInputStream::operator>>(Int8 &d) const
    {
      CheckSize(sizeof(Int8));
      memcpy(&d, m_buffer + m_idx, sizeof(Int8));
      m_idx += sizeof(Int8);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(uint8_t &d) const
    {
      CheckSize(sizeof(uint8_t));
      memcpy(&d, m_buffer + m_idx, sizeof(uint8_t));
      m_idx += sizeof(uint8_t);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(Int16 &d) const
    {
      CheckSize(sizeof(Int16));
      memcpy(&d, m_buffer + m_idx, sizeof(Int16));
      m_idx += sizeof(Int16);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(uint16_t &d) const
    {
      CheckSize(sizeof(uint16_t));
      memcpy(&d, m_buffer + m_idx, sizeof(uint16_t));
      m_idx += sizeof(uint16_t);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(Int32 &d) const
    {
      CheckSize(sizeof(Int32));
      memcpy(&d, m_buffer + m_idx, sizeof(Int32));
      m_idx += sizeof(Int32);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(uint32_t &d) const
    {
      CheckSize(sizeof(uint32_t));
      memcpy(&d, m_buffer + m_idx, sizeof(uint32_t));
      m_idx += sizeof(uint32_t);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(Int64 &d) const
    {
      CheckSize(sizeof(Int64));
      memcpy(&d, m_buffer + m_idx, sizeof(Int64));
      m_idx += sizeof(Int64);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(uint64_t &d) const
    {
      CheckSize(sizeof(uint64_t));
      memcpy(&d, m_buffer + m_idx, sizeof(uint64_t));
      m_idx += sizeof(uint64_t);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(float &d) const
    {
      CheckSize(sizeof(float));
      memcpy(&d, m_buffer + m_idx, sizeof(float));
      m_idx += sizeof(float);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(double &d) const
    {
      CheckSize(sizeof(double));
      memcpy(&d, m_buffer + m_idx, sizeof(double));
      m_idx += sizeof(double);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(bool &d) const
    {
      CheckSize(sizeof(bool));
      memcpy(&d, m_buffer + m_idx, sizeof(bool));
      m_idx += sizeof(bool);
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::operator>>(std::string &d) const
    {
      Size size;
      *this >> size;
      CheckSize(sizeof(char) * d.size());
      d = std::string(reinterpret_cast<const char *>(m_buffer + m_idx), size);
      m_idx += sizeof(char) * d.size();
      return *this;
    }
    
    const BinaryInputStream &BinaryInputStream::Read(uint8_t *buffer, Size size) const
    {
      CheckSize(size);
      memcpy(buffer, m_buffer + m_idx, size);
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
    
    BinaryDictionary::~BinaryDictionary() noexcept
    {
      m_header.clear();
      for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
      {
        if (it->second.m_data)
        {
          delete [] it->second.m_data;
          it->second.m_data = nullptr;
        }
      }
      m_entries.clear();
    }
    
    
    BinaryDictionary::BinaryDictionary(const BinaryInputStream &stream)
    {
      ReadHeader(stream);
    }
    
    void BinaryDictionary::Write(BinaryOutputStream &stream) const
    {
      stream << m_entries.size();
      std::map<std::string, Size> bufferPositions;
      for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
      {
        stream << it->first;
        bufferPositions[it->first] = stream.Tell();
        stream << (Size) 0;
      }
      
      for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
      {
        Size current = stream.Tell();
        
        Size bufferPos = bufferPositions[it->first];
        stream.Seek(bufferPos);
        stream << current;
        stream.Seek(current);
        stream << it->second.m_dataSize;
        stream.Write(it->second.m_data, it->second.m_dataSize);
      }
    }
    
    void BinaryDictionary::ReadAll(const BinaryInputStream &stream)
    {
      ReadHeader(stream);
      
      for (auto it = m_header.begin(); it != m_header.end(); ++it)
      {
        ReadEntry(stream, it->first);
      }
    }
    
    void BinaryDictionary::ReadHeader(const BinaryInputStream &stream)
    {
      m_header.clear();
      m_entries.clear();
      Size size;
      stream >> size;
      for (Size i = 0; i < size; i++)
      {
        std::string name;
        Size bufferPosition;
        stream >> name
               >> bufferPosition;
        
        m_header[name] = bufferPosition;
      }
    }
    
    void BinaryDictionary::ReadEntry(const BinaryInputStream &stream, const std::string &name)
    {
      if (m_entries.find(name) != m_entries.end())
      {
        return;
      }
      
      auto header = m_header.find(name);
      if (header == m_header.end())
      {
        return;
      }
      
      stream.Seek(header->second);
      Size dataSize;
      stream >> dataSize;
      _Entry entry;
      entry.Name = name;
      entry.m_dataSize = dataSize;
      entry.m_data = new uint8_t[entry.m_dataSize];
      stream.Read(entry.m_data, entry.m_dataSize);
      m_entries[name] = entry;
    }
    
    const BinaryDictionary::Entry BinaryDictionary::Read(const BinaryInputStream &stream, const std::string &name)
    {
      ReadEntry(stream, name);
      auto entry = m_entries.find(name);
      if (entry == m_entries.end())
      {
        return Entry(name, nullptr, 0);
      }
      return Entry(name, entry->second.m_data, entry->second.m_dataSize);
    }
    
    void BinaryDictionary::Put(const std::string &name, const uint8_t *data, Size dataSize)
    {
      if (Contains(name))
      {
        throw BinaryEntryAlreadyExistsException();
      }
      
      _Entry entry;
      entry.Name = name;
      entry.m_data = new uint8_t[dataSize];
      entry.m_dataSize = dataSize;
      memcpy(entry.m_data, data, dataSize);
      m_entries[name] = entry;
      
      
    }
    
    bool BinaryDictionary::Remove(const std::string &name)
    {
      auto it = m_entries.find(name);
      if (it == m_entries.end())
      {
        return false;
      }
      
      delete[] it->second.m_data;
      m_entries.erase(it);
      return true;
    }
    
    
    bool BinaryDictionary::Contains(const std::string &name) const
    {
      return m_entries.find(name) != m_entries.end();
    }
    
    const BinaryDictionary::Entry BinaryDictionary::Get(const std::string &name) const
    {
      auto it = m_entries.find(name);
      if (it != m_entries.end())
      {
        return Entry(name, it->second.m_data, it->second.m_dataSize);
      }
      return Entry(name, nullptr, 0);
    }
  
  
}
