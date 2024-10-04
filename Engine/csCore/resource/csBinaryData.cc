

#include <csCore/resource/csBinaryData.hh>

namespace cryo
{
    
 
/* ************************************************************************
 * ************************************************************************
 * 	Binary Output Stream
 * ************************************************************************
 * ************************************************************************
 */
    
    csBinaryOutputStream::csBinaryOutputStream()
      : m_buffer(nullptr)
      , m_capacity(0)
      , m_idx(0)
      , m_sizeIncrement(256)
    {
    
    }
    
    csBinaryOutputStream::~csBinaryOutputStream()
    {
      if (m_buffer)
      {
        delete[] m_buffer;
        m_buffer = nullptr;
      }
      
      m_capacity = 0;
      m_idx = 0;
    }
    
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const int8_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const uint8_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const int16_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const uint16_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const int32_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const uint32_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const int64_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const uint64_t d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const float d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const double d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const bool d)
    {
      Reserve(sizeof(d));
      memcpy(m_buffer + m_idx, &d, sizeof(d));
      m_idx += sizeof(d);
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::operator<<(const std::string &d)
    {
      Size size = d.size();
      *this << size;
      Reserve(sizeof(char) * d.size());
      memcpy(m_buffer + m_idx, d.c_str(), sizeof(char) * d.size());
      m_idx += sizeof(char) * d.size();
      return *this;
    }
    
    csBinaryOutputStream &csBinaryOutputStream::Write(const uint8_t *data, Size size)
    {
      Reserve(size);
      memcpy(m_buffer + m_idx, data, size);
      m_idx += size;
      return *this;
    }
    
    
    void csBinaryOutputStream::Reserve(Size size)
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
    
    
    csBinaryInputStream::csBinaryInputStream(const uint8_t *buffer, Size bufferSize)
      : m_buffer(buffer)
      , m_bufferSize(bufferSize)
      , m_idx(0)
    {
    
    }
    
    csBinaryInputStream::~csBinaryInputStream()
    {
      m_buffer = nullptr;
      m_bufferSize = 0;
      m_idx = 0;
    }
    
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(int8_t &d) const
    {
      CheckSize(sizeof(int8_t));
      memcpy(&d, m_buffer + m_idx, sizeof(int8_t));
      m_idx += sizeof(int8_t);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(uint8_t &d) const
    {
      CheckSize(sizeof(uint8_t));
      memcpy(&d, m_buffer + m_idx, sizeof(uint8_t));
      m_idx += sizeof(uint8_t);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(int16_t &d) const
    {
      CheckSize(sizeof(int16_t));
      memcpy(&d, m_buffer + m_idx, sizeof(int16_t));
      m_idx += sizeof(int16_t);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(uint16_t &d) const
    {
      CheckSize(sizeof(uint16_t));
      memcpy(&d, m_buffer + m_idx, sizeof(uint16_t));
      m_idx += sizeof(uint16_t);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(int32_t &d) const
    {
      CheckSize(sizeof(int32_t));
      memcpy(&d, m_buffer + m_idx, sizeof(int32_t));
      m_idx += sizeof(int32_t);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(uint32_t &d) const
    {
      CheckSize(sizeof(uint32_t));
      memcpy(&d, m_buffer + m_idx, sizeof(uint32_t));
      m_idx += sizeof(uint32_t);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(int64_t &d) const
    {
      CheckSize(sizeof(int64_t));
      memcpy(&d, m_buffer + m_idx, sizeof(int64_t));
      m_idx += sizeof(int64_t);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(uint64_t &d) const
    {
      CheckSize(sizeof(uint64_t));
      memcpy(&d, m_buffer + m_idx, sizeof(uint64_t));
      m_idx += sizeof(uint64_t);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(float &d) const
    {
      CheckSize(sizeof(float));
      memcpy(&d, m_buffer + m_idx, sizeof(float));
      m_idx += sizeof(float);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(double &d) const
    {
      CheckSize(sizeof(double));
      memcpy(&d, m_buffer + m_idx, sizeof(double));
      m_idx += sizeof(double);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(bool &d) const
    {
      CheckSize(sizeof(bool));
      memcpy(&d, m_buffer + m_idx, sizeof(bool));
      m_idx += sizeof(bool);
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::operator>>(std::string &d) const
    {
      Size size;
      *this >> size;
      CheckSize(sizeof(char) * d.size());
      d = std::string(reinterpret_cast<const char *>(m_buffer + m_idx), size);
      m_idx += sizeof(char) * d.size();
      return *this;
    }
    
    const csBinaryInputStream &csBinaryInputStream::Read(uint8_t *buffer, Size size) const
    {
      CheckSize(size);
      memcpy(buffer, m_buffer + m_idx, size);
      m_idx += size;
      return *this;
    }
    
    void csBinaryInputStream::CheckSize(Size size) const
    {
      if (m_idx + size > m_bufferSize)
      {
        throw csBufferExhaustedException();
      }
    }
    
    
    csBinaryDictionary::csBinaryDictionary()
    {
    
    }
    
    csBinaryDictionary::~csBinaryDictionary() noexcept
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
    
    
    csBinaryDictionary::csBinaryDictionary(const csBinaryInputStream &stream)
    {
      ReadHeader(stream);
    }
    
    void csBinaryDictionary::Write(csBinaryOutputStream &stream) const
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
    
    void csBinaryDictionary::ReadAll(const csBinaryInputStream &stream)
    {
      ReadHeader(stream);
      
      for (auto it = m_header.begin(); it != m_header.end(); ++it)
      {
        ReadEntry(stream, it->first);
      }
    }
    
    void csBinaryDictionary::ReadHeader(const csBinaryInputStream &stream)
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
    
    void csBinaryDictionary::ReadEntry(const csBinaryInputStream &stream, const std::string &name)
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
    
    const csBinaryDictionary::Entry csBinaryDictionary::Read(const csBinaryInputStream &stream, const std::string &name)
    {
      ReadEntry(stream, name);
      auto entry = m_entries.find(name);
      if (entry == m_entries.end())
      {
        return Entry(name, nullptr, 0);
      }
      return Entry(name, entry->second.m_data, entry->second.m_dataSize);
    }
    
    void csBinaryDictionary::Put(const std::string &name, const uint8_t *data, Size dataSize)
    {
      if (Contains(name))
      {
        throw csBinaryEntryAlreadyExistsException();
      }
      
      _Entry entry;
      entry.Name = name;
      entry.m_data = new uint8_t[dataSize];
      entry.m_dataSize = dataSize;
      memcpy(entry.m_data, data, dataSize);
      m_entries[name] = entry;
      
      
    }
    
    bool csBinaryDictionary::Remove(const std::string &name)
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
    
    
    bool csBinaryDictionary::Contains(const std::string &name) const
    {
      return m_entries.find(name) != m_entries.end();
    }
    
    const csBinaryDictionary::Entry csBinaryDictionary::Get(const std::string &name) const
    {
      auto it = m_entries.find(name);
      if (it != m_entries.end())
      {
        return Entry(name, it->second.m_data, it->second.m_dataSize);
      }
      return Entry(name, nullptr, 0);
    }
  
  
}
