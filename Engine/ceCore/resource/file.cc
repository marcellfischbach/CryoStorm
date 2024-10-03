
#include <ceCore/resource/file.hh>
#include <ceCore/resource/ifile.hh>
#include <stdio.h>


namespace cryo
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
  {}

  Token(TokenType type, std::string value)
      : type(type), value(value)
  {}

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

  virtual void ReadRest(char **outBuffer, size_t &size) = 0;
};

class BufferBuffer : public iBuffer
{
public:
  BufferBuffer(const char *buffer, size_t bufferSize);

  ~BufferBuffer() override;

  bool IsEOF() override;

  char GetNext() override;

  void Put() override;

  void ReadRest(char **outBuffer, size_t &size) override;

private:
  const char *m_buffer;
  size_t     m_bufferSize;
  size_t     m_idx;

};

Token GetNextToken(iBuffer *buffer);

CrimsonFileAttribute::CrimsonFileAttribute(const std::string &value, AttributeType type)
    : m_name(""), m_value(value), m_type(type)
{
}

CrimsonFileAttribute::CrimsonFileAttribute(const std::string &name, const std::string &value, AttributeType type)
    : m_name(name), m_value(value), m_type(type)
{
}

const std::string &CrimsonFileAttribute::GetName() const
{
  return m_name;
}

const std::string &CrimsonFileAttribute::GetValue() const
{
  return m_value;
}

int CrimsonFileAttribute::GetIntValue() const
{
  return atoi(m_value.c_str());
}

float CrimsonFileAttribute::GetFloatValue() const
{
  return (float) atof(m_value.c_str());
}


double CrimsonFileAttribute::GetDoubleValue() const
{
  return atof(m_value.c_str());
}

CrimsonFileAttribute::AttributeType CrimsonFileAttribute::GetType() const
{
  return m_type;
}

CrimsonFileElement::CrimsonFileElement()
    : m_parent(nullptr)
{

}

CrimsonFileElement::~CrimsonFileElement()
{
  for (CrimsonFileElement*element: m_children)
  {
    delete element;
  }
  m_children.clear();
  m_parent = nullptr;
}

void CrimsonFileElement::SetTagName(const std::string &tagName)
{
  m_tagName = tagName;
}

const std::string &CrimsonFileElement::GetTagName() const
{
  return m_tagName;
}

void CrimsonFileElement::AddChild(CrimsonFileElement *element)
{
  element->m_parent = this;
  m_children.push_back(element);
}

CrimsonFileElement *CrimsonFileElement::GetParent()
{
  return m_parent;
}

const CrimsonFileElement *CrimsonFileElement::GetParent() const
{
  return m_parent;
}

size_t CrimsonFileElement::GetNumberOfChildren() const
{
  return m_children.size();
}

CrimsonFileElement *CrimsonFileElement::GetChild(size_t idx)
{
  return const_cast<CrimsonFileElement *>(static_cast<const CrimsonFileElement *>(this)->GetChild(idx));
}

const CrimsonFileElement *CrimsonFileElement::GetChild(size_t idx) const
{
  if (idx >= m_children.size())
  {
    return nullptr;
  }
  return m_children[idx];
}

CrimsonFileElement *CrimsonFileElement::GetChild(const std::string &childName)
{
  return const_cast<CrimsonFileElement *>(static_cast<const CrimsonFileElement *>(this)->GetChild(childName));
}

const CrimsonFileElement *CrimsonFileElement::GetChild(const std::string &childName) const
{
  std::string path = childName;
  while (true)
  {
    for (auto child: m_children)
    {
      if (path == child->m_tagName)
      {
        if (path.length() >= childName.length())
        {
          return child;
        }
        else
        {
          std::string             tail     = childName.substr(path.length() + 1);
          const CrimsonFileElement*element = child->GetChild(tail);
          if (element)
          {
            return element;
          }
        }
      }
    }

    Size idx = path.find_last_of('.');
    if (idx == std::string::npos)
    {
      break;
    }
    path = path.substr(0, idx);
  }
  return nullptr;
}


bool CrimsonFileElement::HasChild(const std::string &childName) const
{
  return GetChild(childName) != nullptr;
}

void CrimsonFileElement::AddAttribute(const CrimsonFileAttribute &attribute)
{
  m_attributes.push_back(attribute);
}

size_t CrimsonFileElement::GetNumberOfAttributes() const
{
  return m_attributes.size();
}

bool CrimsonFileElement::HasAttribute(const std::string &attributeName) const
{
  for (const CrimsonFileAttribute&attr: m_attributes)
  {
    if (attr.GetName() == attributeName)
    {
      return true;
    }
  }
  return false;
}

const CrimsonFileAttribute *CrimsonFileElement::GetAttribute(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return nullptr;
  }
  return &m_attributes[idx];
}

const CrimsonFileAttribute *CrimsonFileElement::GetAttribute(const std::string &attributeName) const
{
  for (const CrimsonFileAttribute&attr: m_attributes)
  {
    if (attr.GetName() == attributeName)
    {
      return &attr;
    }
  }
  return nullptr;
}

const std::string CrimsonFileElement::GetAttribute(size_t idx, const std::string &defaultValue) const
{
  const CrimsonFileAttribute*attr = GetAttribute(idx);
  return attr ? attr->GetValue() : defaultValue;
}


const std::string CrimsonFileElement::GetAttribute(const std::string &attributeName, const std::string &defaultValue) const
{
  const CrimsonFileAttribute*attr = GetAttribute(attributeName);
  return attr ? attr->GetValue() : defaultValue;
}


int CrimsonFileElement::GetAttribute(size_t idx, int defaultValue) const
{
  const CrimsonFileAttribute*attr = GetAttribute(idx);
  return attr ? attr->GetIntValue() : defaultValue;
}


int CrimsonFileElement::GetAttribute(const std::string &attributeName, int defaultValue) const
{
  const CrimsonFileAttribute*attr = GetAttribute(attributeName);
  return attr ? attr->GetIntValue() : defaultValue;
}

float CrimsonFileElement::GetAttribute(size_t idx, float defaultValue) const
{
  const CrimsonFileAttribute*attr = GetAttribute(idx);
  return attr ? attr->GetFloatValue() : defaultValue;
}


float CrimsonFileElement::GetAttribute(const std::string &attributeName, float defaultValue) const
{
  const CrimsonFileAttribute*attr = GetAttribute(attributeName);
  return attr ? attr->GetFloatValue() : defaultValue;
}


double CrimsonFileElement::GetAttribute(size_t idx, double defaultValue) const
{
  const CrimsonFileAttribute*attr = GetAttribute(idx);
  return attr ? attr->GetDoubleValue() : defaultValue;
}


double CrimsonFileElement::GetAttribute(const std::string &attributeName, double defaultValue) const
{
  const CrimsonFileAttribute*attr = GetAttribute(attributeName);
  return attr ? attr->GetDoubleValue() : defaultValue;
}


CrimsonFile::CrimsonFile()
    : m_data(nullptr), m_dataSize(0)
{

}

CrimsonFile::~CrimsonFile()
{
  if (m_data)
  {
    delete[] m_data;
    m_data = nullptr;
  }
  m_dataSize = 0;
}

CrimsonFileElement *CrimsonFile::Root()
{
  return &m_root;
}

const CrimsonFileElement *CrimsonFile::Root() const
{
  return &m_root;
}

const char *CrimsonFile::GetData() const
{
  return m_data;
}

size_t CrimsonFile::GetDataSize() const
{
  return m_dataSize;
}

bool CrimsonFile::Parse(const std::string &filename)
{
#ifdef CS_WIN32
  FILE *file = nullptr;
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

  char *buffer         = new char[size];
  fread(buffer, 1, size, file);
  fclose(file);


  BufferBuffer bBuf(buffer, size);
  bool         success = Parse(&bBuf);
  if (!success)
  {
    printf ("Parse failed %s\n", filename.c_str());
    printf ("%s\n", buffer);
  }
  delete[] buffer;
  return success;
}

bool CrimsonFile::Parse(iFile *file)
{
  if (!file)
  {
    return false;
  }

  file->Seek(eSM_End, 0);
  long size = file->Tell();
  file->Seek(eSM_Set, 0);

  char *buffer = new char[size + 1];
  memset(buffer, 0, sizeof(char) * (size + 1));
  size_t readSize = file->Read(1, size, buffer);
  if (readSize != size)
  {
    printf("%s size: %ld  readSize: %zu\n", file->GetName().c_str(), size, readSize);
  }

  BufferBuffer bBuf(buffer, size);
  bool         success = Parse(&bBuf);
  if (!success)
  {
    printf ("Parse failed: %s\n", file->GetName().c_str());
    printf ("%s\n", buffer);
  }
  delete[] buffer;
  return success;
}

bool CrimsonFile::Parse(const char *buffer, size_t bufferSize)
{
  BufferBuffer bBuf(buffer, bufferSize);
  bool  success = Parse(&bBuf);
  if (!success)
  {
    printf ("Parse failed:\n%s\n", buffer);
  }

  return success;
}


bool CrimsonFile::Parse(iBuffer *buffer)
{
  if (!buffer)
  {
    return false;
  }

  CrimsonFileElement*parent         = &m_root;
  CrimsonFileElement*currentElement = nullptr;
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
        parent         = currentElement;
        currentElement = nullptr;
        break;
      case TokenType::CurlyBraceClose:
        parent         = parent->GetParent();
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
        buffer->ReadRest(&m_data, m_dataSize);
        return true;

      case TokenType::Identifier:
        if (!currentElement)
        {
          currentElement = new CrimsonFileElement();
          currentElement->SetTagName(token.value);
          parent->AddChild(currentElement);
        }
        else
        {
          std::string attributeName = token.value;
          token = GetNextToken(buffer);
          if (token.type != TokenType::Colon)
          {
            currentElement->AddAttribute(CrimsonFileAttribute(attributeName, CrimsonFileAttribute::AttributeType::String));
            revoke = true;
          }
          else
          {
            token = GetNextToken(buffer);
            if (token.type == TokenType::String)
            {
              currentElement->AddAttribute(CrimsonFileAttribute(attributeName, token.value, CrimsonFileAttribute::AttributeType::String));
            }
            else if (token.type == TokenType::Number)
            {
              currentElement->AddAttribute(CrimsonFileAttribute(attributeName, token.value, CrimsonFileAttribute::AttributeType::Number));
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
        currentElement->AddAttribute(CrimsonFileAttribute(token.value, CrimsonFileAttribute::AttributeType::String));
        break;
      case TokenType::Number:
        if (!currentElement)
        {
          printf("No current element for number '%s'\n", token.value.c_str());
          return false;
        }
        currentElement->AddAttribute(CrimsonFileAttribute(token.value, CrimsonFileAttribute::AttributeType::Number));
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


BufferBuffer::BufferBuffer(const char *buffer, size_t bufferSize)
    : m_buffer(buffer), m_bufferSize(bufferSize), m_idx(0)
{

}

BufferBuffer::~BufferBuffer()
{
  m_buffer     = nullptr;
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

void BufferBuffer::ReadRest(char **outBuffer, size_t &bufferSize)
{
  bufferSize = m_bufferSize - m_idx;
  *outBuffer = new char[bufferSize];
  memcpy(*outBuffer, &m_buffer[m_idx], bufferSize);
}


Token GetNextToken(iBuffer *buffer)
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

void Debug(const CrimsonFileElement *element, int indent)
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
    const CrimsonFileAttribute*attr = element->GetAttribute(i);
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
    for (int    i = 0; i < indent; i++)
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

void CrimsonFile::Debug() const
{
  cryo::Debug(&m_root, 0);
}

std::string Print(const CrimsonFileElement *element, bool format, int ind, const std::string &indent, bool &endWithCurly)
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
    const CrimsonFileAttribute*attr = element->GetAttribute(i);
    line += " ";
    if (attr->GetName().length() > 0)
    {
      line += attr->GetName() + ":";
    }
    if (attr->GetType() == CrimsonFileAttribute::AttributeType::String)
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
    endWithCurly  = true;
  }
  return line;
}

std::string CrimsonFile::Print(bool format, int indent)
{
  std::string indentStr;
  for (int    i = 0; i < indent; i++)
  {
    indentStr += " ";
  }
  std::string print;

  for (size_t i = 0, in = m_root.GetNumberOfChildren(); i < in; i++)
  {
    bool ewc = false;
    print += cryo::Print(m_root.GetChild(i), format, 0, indentStr, ewc);
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

}