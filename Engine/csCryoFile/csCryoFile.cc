
#include <csCryoFile/csCryoFile.hh>
#include <stdio.h>
#include <streambuf>


namespace cs::file
{

enum class TokenType
{
  Invalid,
  Identifier,
  CurlyBraceOpen,
  CurlyBraceClose,
  Colon,
  Comma,
  Number,
  String,
  EOD
};

struct Token
{
  TokenType   type;
  std::string value;

  Token(TokenType type)
    : type(type)
  {
  }

  Token(TokenType type, std::string value)
    : type(type), value(value)
  {
  }

  Token(TokenType type, char ch)
    : type(type), value("")
  {
    value += ch;
  }
};


struct iBuffer
{
  virtual ~iBuffer() = default;

  virtual bool IsEOF() = 0;

  virtual char GetNext() = 0;

  virtual void Put() = 0;

  virtual void ReadRest(char** outBuffer, size_t& size) = 0;
};

class BufferBuffer : public iBuffer
{
public:
  BufferBuffer(const char* buffer, size_t bufferSize);

  ~BufferBuffer() override;

  bool IsEOF() override;

  char GetNext() override;

  void Put() override;

  void ReadRest(char** outBuffer, size_t& size) override;

private:
  const char* m_buffer;
  size_t     m_bufferSize;
  size_t     m_idx;

};

Token GetNextToken(iBuffer* buffer);

csCryoFileAttribute::csCryoFileAttribute(const std::string& value, AttributeType type)
  : m_name(""), m_value(value), m_type(type)
{
}

csCryoFileAttribute::csCryoFileAttribute(const std::string& name, const std::string& value, AttributeType type)
  : m_name(name), m_value(value), m_type(type)
{
}

const std::string& csCryoFileAttribute::GetName() const
{
  return m_name;
}

const std::string& csCryoFileAttribute::GetValue() const
{
  return m_value;
}

int csCryoFileAttribute::GetIntValue() const
{
  return atoi(m_value.c_str());
}

float csCryoFileAttribute::GetFloatValue() const
{
  return (float)atof(m_value.c_str());
}


double csCryoFileAttribute::GetDoubleValue() const
{
  return atof(m_value.c_str());
}

csCryoFileAttribute::AttributeType csCryoFileAttribute::GetType() const
{
  return m_type;
}

csCryoFileElement::csCryoFileElement()
  : m_parent(nullptr)
{

}

csCryoFileElement::~csCryoFileElement()
{
  for (csCryoFileElement* element : m_children)
  {
    delete element;
  }
  m_children.clear();
  m_parent = nullptr;
}

void csCryoFileElement::SetTagName(const std::string& tagName)
{
  m_tagName = tagName;
}

const std::string& csCryoFileElement::GetTagName() const
{
  return m_tagName;
}

void csCryoFileElement::AddChild(csCryoFileElement* element)
{
  element->m_parent = this;
  m_children.push_back(element);
}

csCryoFileElement* csCryoFileElement::GetParent()
{
  return m_parent;
}

const csCryoFileElement* csCryoFileElement::GetParent() const
{
  return m_parent;
}

size_t csCryoFileElement::GetNumberOfChildren() const
{
  return m_children.size();
}

csCryoFileElement* csCryoFileElement::GetChild(size_t idx)
{
  return const_cast<csCryoFileElement*>(static_cast<const csCryoFileElement*>(this)->GetChild(idx));
}

const csCryoFileElement* csCryoFileElement::GetChild(size_t idx) const
{
  if (idx >= m_children.size())
  {
    return nullptr;
  }
  return m_children[idx];
}

csCryoFileElement* csCryoFileElement::GetChild(const std::string& childName)
{
  return const_cast<csCryoFileElement*>(static_cast<const csCryoFileElement*>(this)->GetChild(childName));
}

const csCryoFileElement* csCryoFileElement::GetChild(const std::string& childName) const
{
  std::string path = childName;
  while (true)
  {
    for (auto child : m_children)
    {
      if (path == child->m_tagName)
      {
        if (path.length() >= childName.length())
        {
          return child;
        }
        else
        {
          std::string             tail = childName.substr(path.length() + 1);
          const csCryoFileElement* element = child->GetChild(tail);
          if (element)
          {
            return element;
          }
        }
      }
    }

    size_t idx = path.find_last_of('.');
    if (idx == std::string::npos)
    {
      break;
    }
    path = path.substr(0, idx);
  }
  return nullptr;
}


bool csCryoFileElement::HasChild(const std::string& childName) const
{
  return GetChild(childName) != nullptr;
}

void csCryoFileElement::AddAttribute(const csCryoFileAttribute& attribute)
{
  m_attributes.push_back(attribute);
}

size_t csCryoFileElement::GetNumberOfAttributes() const
{
  return m_attributes.size();
}

bool csCryoFileElement::HasAttribute(const std::string& attributeName) const
{
  for (const csCryoFileAttribute& attr : m_attributes)
  {
    if (attr.GetName() == attributeName)
    {
      return true;
    }
  }
  return false;
}

const csCryoFileAttribute* csCryoFileElement::GetAttribute(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return nullptr;
  }
  return &m_attributes[idx];
}

const csCryoFileAttribute* csCryoFileElement::GetAttribute(const std::string& attributeName) const
{
  for (const csCryoFileAttribute& attr : m_attributes)
  {
    if (attr.GetName() == attributeName)
    {
      return &attr;
    }
  }
  return nullptr;
}

const std::string csCryoFileElement::GetAttribute(size_t idx, const std::string& defaultValue) const
{
  const csCryoFileAttribute* attr = GetAttribute(idx);
  return attr ? attr->GetValue() : defaultValue;
}


const std::string csCryoFileElement::GetAttribute(const std::string& attributeName, const std::string& defaultValue) const
{
  const csCryoFileAttribute* attr = GetAttribute(attributeName);
  return attr ? attr->GetValue() : defaultValue;
}


int csCryoFileElement::GetAttribute(size_t idx, int defaultValue) const
{
  const csCryoFileAttribute* attr = GetAttribute(idx);
  return attr ? attr->GetIntValue() : defaultValue;
}


int csCryoFileElement::GetAttribute(const std::string& attributeName, int defaultValue) const
{
  const csCryoFileAttribute* attr = GetAttribute(attributeName);
  return attr ? attr->GetIntValue() : defaultValue;
}

float csCryoFileElement::GetAttribute(size_t idx, float defaultValue) const
{
  const csCryoFileAttribute* attr = GetAttribute(idx);
  return attr ? attr->GetFloatValue() : defaultValue;
}


float csCryoFileElement::GetAttribute(const std::string& attributeName, float defaultValue) const
{
  const csCryoFileAttribute* attr = GetAttribute(attributeName);
  return attr ? attr->GetFloatValue() : defaultValue;
}


double csCryoFileElement::GetAttribute(size_t idx, double defaultValue) const
{
  const csCryoFileAttribute* attr = GetAttribute(idx);
  return attr ? attr->GetDoubleValue() : defaultValue;
}


double csCryoFileElement::GetAttribute(const std::string& attributeName, double defaultValue) const
{
  const csCryoFileAttribute* attr = GetAttribute(attributeName);
  return attr ? attr->GetDoubleValue() : defaultValue;
}


csCryoFile::csCryoFile()
  : m_data(nullptr), m_dataSize(0)
{

}

csCryoFile::~csCryoFile()
{
  if (m_data)
  {
    delete[] m_data;
    m_data = nullptr;
  }
  m_dataSize = 0;
}

csCryoFileElement* csCryoFile::Root()
{
  return &m_root;
}

const csCryoFileElement* csCryoFile::Root() const
{
  return &m_root;
}

const char* csCryoFile::GetData() const
{
  return m_data;
}

size_t csCryoFile::GetDataSize() const
{
  return m_dataSize;
}

static std::vector<uint8_t> NULL_DATA;

std::vector<std::string> csCryoFile::GetDataNames() const
{
  std::vector<std::string> names;
  for (auto data : m_datas)
  {
    names.push_back(data.name);
  }
  return names;
}

size_t csCryoFile::GetNumberOfDatas() const
{
  return m_datas.size();
}

const std::vector<uint8_t>& csCryoFile::GetData(size_t idx) const
{
  if (idx >= m_datas.size())
  {
    return NULL_DATA;
  }
  return m_datas[idx].data;
}

const std::vector<uint8_t>& csCryoFile::GetData(const std::string& name) const
{
  for (auto& data : m_datas)
  {
    if (data.name == name)
    {
      return data.data;
    }
  }
  return NULL_DATA;
}


void csCryoFile::AddData(const std::string& name, uint32_t size, uint8_t* data)
{
  std::vector<uint8_t> dataBuffer;
  dataBuffer.resize(size);
  memcpy(dataBuffer.data(), data, size);

  m_datas.emplace_back(name, dataBuffer);
}


void csCryoFile::AddData(const std::string& name, std::vector<uint8_t> data)
{
  m_datas.emplace_back(name, data);
}


bool csCryoFile::Parse(const std::string& filename)
{
#ifdef CS_WIN32
  FILE* file = nullptr;
  if (fopen_s(&file, filename.c_str(), "rb") != 0)
  {
    return false;
  }
#else
  FILE* file = fopen(filename.c_str(), "rb");
#endif
  if (!file)
  {
    return false;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = new char[size];
  fread(buffer, 1, size, file);
  fclose(file);


  BufferBuffer bBuf(buffer, size);
  bool         success = Parse(&bBuf);
  if (!success)
  {
    printf("Parse failed %s\n", filename.c_str());
    printf("%s\n", buffer);
  }
  delete[] buffer;
  return success;
}

bool csCryoFile::Parse(const std::vector<char>& data)
{

  BufferBuffer bBuf(data.data(), data.size());
  return Parse(&bBuf);
}

bool csCryoFile::Parse(const char* buffer, size_t bufferSize)
{
  BufferBuffer bBuf(buffer, bufferSize);
  bool  success = Parse(&bBuf);
  if (!success)
  {
    printf("Parse failed:\n%s\n", buffer);
  }

  return success;
}


bool csCryoFile::Parse(iBuffer* buffer)
{
  if (!buffer)
  {
    return false;
  }

  csCryoFileElement* parent = &m_root;
  csCryoFileElement* currentElement = nullptr;
  while (true)
  {
    Token token = GetNextToken(buffer);
    if (token.type == TokenType::Invalid)
    {
      break;
    }
    while (true)
    {

      bool revoke = false;
      switch (token.type)
      {
      case TokenType::CurlyBraceOpen:
        parent = currentElement;
        currentElement = nullptr;
        break;
      case TokenType::CurlyBraceClose:
        parent = parent->GetParent();
        currentElement = nullptr;
        break;
      case TokenType::Comma:
        currentElement = nullptr;
        break;
      case TokenType::Colon:
        printf("Invalid token colon\n");
        return false;
      case TokenType::EOD:
        if (parent != &m_root || currentElement != nullptr)
        {
          printf("Unexpected End of Document found\n");
          return false;
        }
        else
        {
          char* dataBuffer = nullptr;
          size_t dataSize= 0;
          buffer->ReadRest(&dataBuffer, dataSize);
          ParseData(dataSize, dataBuffer);
          delete[] dataBuffer;
          return true;
        }

      case TokenType::Identifier:
        if (!currentElement)
        {
          currentElement = new csCryoFileElement();
          currentElement->SetTagName(token.value);
          parent->AddChild(currentElement);
        }
        else
        {
          std::string attributeName = token.value;
          token = GetNextToken(buffer);
          if (token.type != TokenType::Colon)
          {
            currentElement->AddAttribute(csCryoFileAttribute(attributeName, csCryoFileAttribute::AttributeType::String));
            revoke = true;
          }
          else
          {
            token = GetNextToken(buffer);
            if (token.type == TokenType::String)
            {
              currentElement->AddAttribute(csCryoFileAttribute(attributeName, token.value, csCryoFileAttribute::AttributeType::String));
            }
            else if (token.type == TokenType::Number)
            {
              currentElement->AddAttribute(csCryoFileAttribute(attributeName, token.value, csCryoFileAttribute::AttributeType::Number));
            }
            else
            {
              printf("Expecting string or number\n");
              return false;
            }
          }
        }
        break;

      case TokenType::String:
        if (!currentElement)
        {
          printf("No current element for string '%s'\n", token.value.c_str());
          return false;
        }
        currentElement->AddAttribute(csCryoFileAttribute(token.value, csCryoFileAttribute::AttributeType::String));
        break;
      case TokenType::Number:
        if (!currentElement)
        {
          printf("No current element for number '%s'\n", token.value.c_str());
          return false;
        }
        currentElement->AddAttribute(csCryoFileAttribute(token.value, csCryoFileAttribute::AttributeType::Number));
        break;
      }
      if (!revoke)
      {
        break;
      }
    }

  }

  return true;
}



BufferBuffer::BufferBuffer(const char* buffer, size_t bufferSize)
  : m_buffer(buffer), m_bufferSize(bufferSize), m_idx(0)
{

}

BufferBuffer::~BufferBuffer()
{
  m_buffer = nullptr;
  m_bufferSize = 0;
}

bool BufferBuffer::IsEOF()
{
  return m_idx >= m_bufferSize;
}

char BufferBuffer::GetNext()
{
  size_t idx = m_idx;
  m_idx++;
  return m_buffer[idx];
}

void BufferBuffer::Put()
{
  if (m_idx > 0)
  {
    m_idx--;
  }
}

void BufferBuffer::ReadRest(char** outBuffer, size_t& bufferSize)
{
  bufferSize = m_bufferSize - m_idx;
  *outBuffer = new char[bufferSize];
  memcpy(*outBuffer, &m_buffer[m_idx], bufferSize);
}


Token GetNextToken(iBuffer* buffer)
{

  char ch = ' ';
  while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
  {
    if (buffer->IsEOF())
    {
      return Token(TokenType::Invalid);
    }

    ch = buffer->GetNext();
  }
  while (ch == '#')
  {

    // Skip to the end of the line
    while (ch != '\n')
    {
      if (buffer->IsEOF())
      {
        return Token(TokenType::Invalid);
      }

      ch = buffer->GetNext();
    }
    // now skip all the trailing new lines
    while (ch == '\r' || ch == '\n')
    {
      if (buffer->IsEOF())
      {
        return Token(TokenType::Invalid);
      }

      ch = buffer->GetNext();
    }
  }


  switch (ch)
  {
  case '{':
    return Token(TokenType::CurlyBraceOpen, ch);
  case '}':
    return Token(TokenType::CurlyBraceClose, ch);
  case ',':
    return Token(TokenType::Comma, ch);
  case ':':
    return Token(TokenType::Colon, ch);
  case '@':
    return Token(TokenType::EOD, ch);
  }


  // check identifier
  if (ch == '_'
    || ch >= 'a' && ch <= 'z'
    || ch >= 'A' && ch <= 'Z')
  {
    std::string id;
    id += ch;
    while (!buffer->IsEOF())
    {
      ch = buffer->GetNext();
      if (ch == '_'
        || ch == '.'
        || ch >= 'a' && ch <= 'z'
        || ch >= 'A' && ch <= 'Z'
        || ch >= '0' && ch <= '9')
      {
        id += ch;
      }
      else
      {
        buffer->Put();
        break;
      }
    }
    return Token(TokenType::Identifier, id);
  }

  if (ch == '"')
  {
    std::string str;
    while (!buffer->IsEOF())
    {
      ch = buffer->GetNext();
      if (ch == '\\')
      {
        if (buffer->IsEOF())
        {
          return Token(TokenType::Invalid);
        }
        ch = buffer->GetNext();
        str += ch;
      }
      else if (ch == '"')
      {
        return Token(TokenType::String, str);
      }
      else
      {
        str += ch;
      }
    }
  }

  if (ch >= '0' && ch <= '9' || ch == '-' || ch == '+')
  {
    std::string num;
    num += ch;
    while (!buffer->IsEOF())
    {
      ch = buffer->GetNext();
      if (ch >= '0' && ch <= '9'
        || ch == '-'
        || ch == '+'
        || ch == 'e'
        || ch == 'E'
        || ch == '.'
        )
      {
        num += ch;
      }
      else
      {
        buffer->Put();
        return Token(TokenType::Number, num);
      }
    }
    return Token(TokenType::Number, num);
  }


  return Token(TokenType::Invalid);
}

void Debug(const csCryoFileElement* element, int indent)
{
  for (int i = 0; i < indent; i++)
  {
    printf("  ");
  }

  if (element->GetTagName().length() > 0)
  {
    printf("%s ", element->GetTagName().c_str());
  }
  for (size_t i = 0; i < element->GetNumberOfAttributes(); i++)
  {
    const csCryoFileAttribute* attr = element->GetAttribute(i);
    if (attr->GetName().length() > 0)
    {
      printf("<%s:%s> ", attr->GetName().c_str(), attr->GetValue().c_str());
    }
    else
    {
      printf("<%s> ", attr->GetValue().c_str());
    }
  }

  if (element->GetNumberOfChildren() > 0)
  {
    printf("{\n");
    for (size_t i = 0; i < element->GetNumberOfChildren(); i++)
    {
      Debug(element->GetChild(i), indent + 1);
    }
    for (int i = 0; i < indent; i++)
    {
      printf("  ");
    }
    printf("}\n");
  }
  else
  {
    printf("\n");
  }
}

void csCryoFile::Debug() const
{
  cs::file::Debug(&m_root, 0);
}

std::string Print(const csCryoFileElement* element, bool format, int ind, const std::string& indent, bool& endWithCurly)
{
  endWithCurly = false;
  std::string line;
  if (format)
  {
    for (int i = 0; i < ind; i++)
    {
      line += indent;
    }
  }

  line += element->GetTagName();
  for (size_t i = 0; i < element->GetNumberOfAttributes(); i++)
  {
    const csCryoFileAttribute* attr = element->GetAttribute(i);
    line += " ";
    if (attr->GetName().length() > 0)
    {
      line += attr->GetName() + ":";
    }
    if (attr->GetType() == csCryoFileAttribute::AttributeType::String)
    {
      line += "\"" + attr->GetValue() + "\"";
    }
    else
    {
      line += attr->GetValue();
    }
  }

  if (element->GetNumberOfChildren() > 0)
  {
    if (format)
    {
      line += " {\n";
    }
    else
    {
      line += "{";
    }

    for (size_t i = 0, in = element->GetNumberOfChildren(); i < in; i++)
    {
      bool ewc = false;
      line += Print(element->GetChild(i), format, ind + 1, indent, ewc);
      if (!ewc && i + 1 < in)
      {
        line += ",";
      }
      if (format)
      {
        line += "\n";
      }
    }
    if (format)
    {
      for (int i = 0; i < ind; i++)
      {
        line += indent;
      }
    }
    line += "}";
    endWithCurly = true;
  }
  return line;
}

std::string csCryoFile::ToString2(bool format, int indent)
{
  std::string indentStr;
  for (int i = 0; i < indent; i++)
  {
    indentStr += " ";
  }
  std::string print;

  for (size_t i = 0, in = m_root.GetNumberOfChildren(); i < in; i++)
  {
    bool ewc = false;
    print += cs::file::Print(m_root.GetChild(i), format, 0, indentStr, ewc);
    if (!ewc && i + 1 < in)
    {
      print += ",";
    }
    if (format)
    {
      print += "\n";
    }
  }
  if (m_data && m_dataSize)
  {
    print += "@" + std::string(m_data, m_dataSize);
  }

  return print;

}

void csCryoFile::Write(std::ostream& out, bool format, unsigned indent)
{

  for (size_t i = 0, in = m_root.GetNumberOfChildren(); i < in; i++)
  {
    auto child = m_root.GetChild(i);
    child->Write(out, format, "", indent);
  }

  if (!m_datas.empty())
  {
    WriteData(out);
  }
}

std::string indent_str(unsigned depth)
{
  std::string res;
  res.resize(depth, ' ');
  return res;
}

void csCryoFileElement::Write(std::ostream& out, bool format, const std::string& indent, unsigned indentDepth)
{
  if (format)
  {
    out << indent;
  }
  out << m_tagName;

  // write attributes
  for (auto attrib : m_attributes)
  {
    out << " ";
    if (!attrib.GetName().empty())
    {
      out << attrib.GetName() << ":";
    }
    if (attrib.GetType() == csCryoFileAttribute::AttributeType::String)
    {
      out << '"';
    }
    out << attrib.GetValue();
    if (attrib.GetType() == csCryoFileAttribute::AttributeType::String)
    {
      out << '"';
    }
  }

  if (!m_children.empty())
  {
    out << " {";
    if (format)
    {
      out << std::endl;
    }

    std::string totalIndent = indent + indent_str(indentDepth);
    for (auto child : m_children)
    {
      child->Write(out, format, totalIndent, indentDepth);
    }
    if (format)
    {
      out << indent;
    }
    out << "}";
  }
  out << ",";
  if (format)
  {
    out << std::endl;
  }
}


void write_string(std::ostream& out, const std::string& str)
{
  uint32_t strLength = str.length();
  out.write(reinterpret_cast<char*>(&strLength), sizeof(uint32_t));
  out.write(str.c_str(), strLength);
}

std::string read_string(std::istream& in)
{
  uint32_t strLength;
  in.read(reinterpret_cast<char*>(&strLength), sizeof(uint32_t));
  std::string str;
  str.resize(strLength, ' ');
  in.read(str.data(), strLength);
  return str;
}

uint32_t get_string_length(const std::string& str)
{
  return str.length() + sizeof(uint32_t);
}


void csCryoFile::WriteData(std::ostream& out)
{
  out << "@";


  uint32_t version = 0x01; 
  out.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));

  uint32_t numData = m_datas.size();
  out.write(reinterpret_cast<char*>(&numData), sizeof(uint32_t));

  for (auto& data : m_datas)
  {
    write_string(out, data.name);
    uint32_t bufferSize = data.data.size();
    out.write(reinterpret_cast<char*>(&bufferSize), sizeof(uint32_t));
    out.write(reinterpret_cast<char*>(data.data.data()), data.data.size());
  }
}


struct membuf : std::streambuf
{
  membuf(char* begin, char* end) {
    this->setg(begin, begin, end);
  }
};

void csCryoFile::ParseData(size_t size, char* buffer)
{
  membuf sbuf(buffer, buffer + size);
  std::istream in(&sbuf);

  uint32_t version;
  in.read(reinterpret_cast<char*>(&version), sizeof(uint32_t));
  if (version == 0x01)
  {
    ParseData_V1(in);
  }
}

void csCryoFile::ParseData_V1(std::istream &in)
{
  uint32_t numData;
  in.read(reinterpret_cast<char*>(&numData), sizeof(uint32_t));

  for (uint32_t i = 0; i < numData; i++)
  {
    std::string name = read_string(in);
    uint32_t dataSize;
    in.read(reinterpret_cast<char*>(&dataSize), sizeof(uint32_t));

    std::vector<uint8_t> data;
    data.resize(dataSize);
    in.read(reinterpret_cast<char*>(data.data()), dataSize);

    m_datas.emplace_back(name, data);
  }
}



}