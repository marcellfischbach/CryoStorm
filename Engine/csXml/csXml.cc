#include <csXml/csXml.hh>
#include <cstdio>
#include <vector>

namespace cryo::xml
{


csDocument *csParser::ParseFilename(const std::string &filename)
{
  FILE *fs;
  auto error = fopen_s(&fs, filename.c_str(), "rt");
  if (error)
  {
    return {};
  }

  fseek(fs, 0, SEEK_END);
  long size = ftell(fs);
  fseek(fs, 0, SEEK_SET);

  char *buffer = new char[size + 1];
  memset(buffer, 0, size + 1);

  fread(buffer, sizeof(char), size, fs);
  fclose(fs);

  std::string content(buffer, size);
  delete[] buffer;

  return ParseContent(content);
}

csDocument *csParser::ParseContent(const std::string &content)
{
  csParser parser(content);
  return parser.Parse();
}

csParser::csParser(const std::string &content)
    : m_content(content)
    , m_idx(0)
{
}


csDocument *csParser::Parse()
{
  Tokenize();

  std::string m_error;

  auto document = new csDocument();

  csElement *currentParent = nullptr;

  csElement *currentElement = nullptr;

#define SKIP_WHITE_SPACE while (m_tokens[i].type == eWhiteSpace) i++
  size_t i = 0;
  while (true)
  {
    if (i >= m_tokens.size())
    {
      if (currentParent)
      {
        m_error = "Invalid hierarchy";
        delete document;
        return nullptr;
      }
      break;
    }
    if (m_tokens[i].type == eAngleBracketOpen)
    {
      i++;
      SKIP_WHITE_SPACE;
      if (m_tokens[i].type == eIdent)
      {
        std::string ident = ReadFullIdent(i);
        if (!currentParent)
        {
          currentElement = document->CreateRoot(ident);
        }
        else
        {
          currentElement = currentParent->CreateChildElement(ident);
        }

        while (true)
        {
          SKIP_WHITE_SPACE;
          if (m_tokens[i].type == eSlash)
          {
            i++;
            if (m_tokens[i].type == eAngleBracketClose)
            {
              currentElement = nullptr;
              break;
            }
            else
            {
              m_error = "Expected '>'";
              delete document;
              return nullptr;
            }
          }
          else if (m_tokens[i].type == eAngleBracketClose)
          {
            currentParent = currentElement;
            currentElement = nullptr;
            break;
          }
          if (m_tokens[i].type == eIdent)
          {
            std::string key = ReadFullIdent(i);
            SKIP_WHITE_SPACE;
            if (m_tokens[i].type == eEquals)
            {
              i++;
              SKIP_WHITE_SPACE;
              if (m_tokens[i].type == eString)
              {
                currentElement->AddAttribute(key, m_tokens[i].content);
                i++;
              }
              else
              {
                m_error = "Expected string";
                delete document;
                return nullptr;
              }
            }
            else
            {
              currentElement->AddAttribute(key, "");
            }
          }

        }
      }
      else if (m_tokens[i].type == eSlash)
      {
        i++;
        SKIP_WHITE_SPACE;
        std::string tag = ReadFullIdent(i);
        if (!currentParent)
        {
          m_error = "Invalid hierarchy";
          delete document;
          return nullptr;
        }
        if (currentParent->GetTagName() != tag)
        {
          m_error = "Expected " + currentParent->GetTagName() + " but got " + tag;
          delete document;
          return nullptr;
        }

        currentParent = currentParent->GetParent();
      }
    }
    else
    {
      if (!currentParent)
      {
        m_error = "Invalid hierarchy";
        delete document;
        return nullptr;
      }
      else
      {
        std::string text;
        while (true)
        {
          if (i >= m_tokens.size())
          {
            m_error = "Invalid hierarchy";
            delete document;
            return nullptr;
          }

          if (m_tokens[i].type == eAngleBracketOpen)
          {
            currentParent->CreateChildText(text);
            i--;
            break;
          }

          text += m_tokens[i].content;
          i++;
        }
      }
    }
    i++;
  }


  return document;
}


std::string csParser::ReadFullIdent(size_t &tokenIdx)
{
  std::string ident;
  for (size_t i = tokenIdx; i < m_tokens.size(); i++)
  {
    if (m_tokens[i].type == eWhiteSpace
        || m_tokens[i].type == eEquals
        || m_tokens[i].type == eSlash
        || m_tokens[i].type == eAngleBracketClose)
    {
      break;
    }

    ident += m_tokens[i].content;
    tokenIdx++;
  }
  return ident;
}


void csParser::Tokenize()
{
  m_tokens.clear();
  while (!IsEOF())
  {
    char ch = Pick();
    if (ch == '=')
    {
      m_tokens.emplace_back(eEquals, std::string(1, ch));
    }
    else if (ch == '<')
    {
      m_tokens.emplace_back(eAngleBracketOpen, std::string(1, ch));
    }
    else if (ch == '>')
    {
      m_tokens.emplace_back(eAngleBracketClose, std::string(1, ch));
    }
    else if (ch == '/')
    {
      m_tokens.emplace_back(eSlash, std::string(1, ch));
    }
    else if (ch == '!')
    {
      m_tokens.emplace_back(eExclamationMark, std::string(1, ch));
    }
    else if (ch == '[')
    {
      m_tokens.emplace_back(eSquareBracketsOpen, std::string(1, ch));
    }
    else if (ch == ']')
    {
      m_tokens.emplace_back(eSquareBracketsClose, std::string(1, ch));
    }
    else
    {
      Put();
      ReadOtherToken();
    }
  }
}


void csParser::ReadOtherToken()
{
  char ch = Pick();
  if (ch == '"')
  {
    std::string content;
    while (true)
    {
      if (IsEOF())
      {
        return;
      }

      ch = Pick();
      if (ch == '"')
      {
        break;
      }
      else if (ch == '\\')
      {
        if (IsEOF())
        {
          return;
        }
        ch = Pick();
      }
      content += ch;

    }

    m_tokens.emplace_back(eString, content);
  }
  else if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_')
  {
    Put();
    std::string content;
    while (true)
    {
      if (IsEOF())
      {
        return;
      }

      ch = Pick();
      if (!(ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_' || ch >= '0' && ch <= '9'))
      {
        Put();
        break;
      }
      content += ch;
    }

    m_tokens.emplace_back(eIdent, content);
  }
  else if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
  {
    Put();
    std::string content;
    while (true)
    {
      if (IsEOF())
      {
        return;
      }

      ch = Pick();
      if (!(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'))
      {
        Put();
        break;
      }
      content += ch;
    }

    m_tokens.emplace_back(eWhiteSpace, content);
  }
  else
  {
    if (ch != 0)
    {
      m_tokens.emplace_back(eOther, std::string(1, ch));
    }
  }
}


bool csParser::IsEOF() const
{
  return m_idx >= m_content.length();
}


char csParser::Pick()
{
  char ch = m_content[m_idx];
  m_idx++;
  return ch;
}

void csParser::Put()
{
  if (m_idx >= 1)
  {
    m_idx--;
  }
}


csAttribute::csAttribute(std::string key)
    : m_key(std::move(key))
{

}

const std::string &csAttribute::GetKey() const
{
  return m_key;
}

void csAttribute::SetValue(const std::string &value)
{
  m_value = value;
}

const std::string &csAttribute::GetValue() const
{
  return m_value;
}

csNode::csNode(eNodeType type)
    : m_type(type)
{

}

eNodeType csNode::GetType() const
{
  return m_type;
}


const csElement *csNode::GetParent() const
{
  return m_parent;
}

csElement *csNode::GetParent()
{
  return m_parent;
}


csElement::csElement(std::string tagName)
    : csNode(eNT_Element)
    , m_tagName(std::move(tagName))
{

}

csElement::~csElement()
{
  for (auto child: m_children)
  {
    delete child;
  }
  m_children.clear();

  for (auto item: m_attributes)
  {
    delete item;
  }
  m_attributes.clear();
}

const std::string &csElement::GetTagName() const
{
  return m_tagName;
}

csElement *csElement::CreateChildElement(const std::string &tagName)
{
  auto child = new csElement(tagName);
  child->m_parent = this;
  m_children.push_back(child);
  return child;
}


csText *csElement::CreateChildText(const std::string &text)
{
  auto child = new csText();
  child->SetContent(text);
  child->m_parent = this;
  m_children.push_back(child);
  return child;
}

void csElement::AddAttribute(const std::string &key, const std::string &value)
{
  for (const auto &item: m_attributes)
  {
    if (item->GetKey() == key)
    {
      item->SetValue(value);
      return;
    }
  }

  auto *attrib = new csAttribute(key);
  attrib->SetValue(value);
  m_attributes.push_back(attrib);
}


csText::csText()
    : csNode(eNT_Text)
{

}

void csText::SetContent(const std::string &content)
{
  m_content = content;
}

const std::string &csText::GetContent() const
{
  return m_content;
}

csElement *csDocument::CreateRoot(const std::string &tagName)
{
  if (m_root)
  {
    return m_root;
  }

  m_root = new csElement(tagName);
  m_root->m_parent = nullptr;

  return m_root;
}

const csElement *csDocument::GetRoot() const
{
  return m_root;
}


csDocument::~csDocument()
{
  delete m_root;
  m_root = nullptr;
}

}