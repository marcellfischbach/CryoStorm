#include <csXml/csXml.hh>
#include <cstdio>
#include <vector>
#include <Windows.h>

namespace cryo::xml
{




csParseException::csParseException(const std::string &message, size_t line, size_t column)
    : std::exception(message.c_str())
    , m_line(line)
    , m_column(column)
{
}

size_t csParseException::GetLine() const
{
  return m_line;
}

size_t csParseException::GetColumn() const
{
  return m_column;
}

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

  auto document = new csDocument();

  try
  {
    csElement *currentParent = nullptr;


#define SKIP_WHITE_SPACE while (m_tokens[i].type == eWhiteSpace) i++
    size_t i = 0;
    while (true)
    {
      if (i >= m_tokens.size())
      {
        if (currentParent)
        {
          throw csParseException("Last tag not closed", 0, 0);
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

          csElement *currentElement = nullptr;
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
                throw csParseException("Expected '>'", m_tokens[i].line, m_tokens[i].column);
              }
            }
            else if (m_tokens[i].type == eAngleBracketClose)
            {
              currentParent  = currentElement;
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
                  throw csParseException("Expected string starting with'\"'", m_tokens[i].line, m_tokens[i].column);
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
          size_t openBrackets = i-1;
          i++;
          SKIP_WHITE_SPACE;
          std::string tag = ReadFullIdent(i);
          if (!currentParent)
          {
            throw csParseException("Found invalid closing tag '" + tag + "'. No parent",
                                   m_tokens[openBrackets].line,
                                   m_tokens[openBrackets].column);
          }
          if (currentParent->GetTagName() != tag)
          {
            throw csParseException(
                "Found invalid closing tag '" + tag + "'. '" + currentParent->GetTagName() + "' was expected.",
                m_tokens[openBrackets].line,
                m_tokens[openBrackets].column);
          }

          currentParent = currentParent->GetParent();
        }
        else if (m_tokens[i].type == eExclamationMark)
        {
          if ((i + 1) >= m_tokens.size())
          {
            throw csParseException("Expected '[' or '-'", m_tokens[i].line, m_tokens[i].column + 1);
          }

          if (m_tokens[i + 1].type == eSquareBracketsOpen)
          {
            if ((i + 2) >= m_tokens.size())
            {
              throw csParseException("Expected 'CDATA'", m_tokens[i + 1].line, m_tokens[i + 1].column + 1);
            }
            if (m_tokens[i + 2].type != eIdent || m_tokens[i + 2].content != std::string("CDATA"))
            {
              throw csParseException("Expected 'CDATA'", m_tokens[i + 1].line, m_tokens[i + 1].column);
            }
            if ((i + 3) >= m_tokens.size())
            {
              throw csParseException("Expected '['", m_tokens[i].line, m_tokens[i].column + 1);
            }
            if (m_tokens[i + 3].type != eSquareBracketsOpen)
            {
              throw csParseException("Expected '['", m_tokens[i + 1].line, m_tokens[i + 1].column);
            }
            if (!currentParent)
            {
              throw csParseException("Found CDATA at the beginning", m_tokens[i].line, m_tokens[i].column);
            }

            i += 4;
            std::string cdata;
            while (true)
            {
              if (i >= m_tokens.size())
              {
                throw csParseException("No ending CDATA ']]>' found", m_tokens[i - 1].line, m_tokens[i - 1].column + 1);
              }
              if (i + 2 < m_tokens.size()
                  && m_tokens[i].type == eSquareBracketsClose
                  && m_tokens[i + 1].type == eSquareBracketsClose
                  && m_tokens[i + 2].type == eAngleBracketClose)
              {
                // end of cdata
                currentParent->CreateChildText(cdata, false);
                i += 2;
                break;
              }
              cdata += m_tokens[i].content;
              i++;
            }
          }
          else if (m_tokens[i + 1].type == eDash)
          {
            if ((i + 2) >= m_tokens.size())
            {
              throw csParseException("Expected '-'", m_tokens[i + 1].line, m_tokens[i + 1].column + 1);
            }
            if (m_tokens[i + 2].type != eDash)
            {
              throw csParseException("Expected '-'", m_tokens[i + 1].line, m_tokens[i + 1].column);
            }
            if (!currentParent)
            {
              throw csParseException("Found comment at the beginning", m_tokens[i].line, m_tokens[i].column);
            }

            i += 3;
            std::string comment;
            while (true)
            {
              if (i >= m_tokens.size())
              {
                throw csParseException("No ending '-->' found", m_tokens[i - 1].line, m_tokens[i - 1].column + 1);
              }
              if (i + 2 < m_tokens.size()
                  && m_tokens[i].type == eDash
                  && m_tokens[i + 1].type == eDash
                  && m_tokens[i + 2].type == eAngleBracketClose)
              {
                // end of comment
                currentParent->CreateChildComment(comment);
                i += 2;
                break;
              }
              comment += m_tokens[i].content;
              i++;
            }
          }
          else
          {
            throw csParseException("Expected '[' or '-'", m_tokens[i].line, m_tokens[i].column + 1);
          }


        }
        else
        {
          throw csParseException("Invalid character found. Expected 'IDENT', '/' or '!'",
                                 m_tokens[i].line,
                                 m_tokens[i].column);
        }
      }
      else
      {
        if (!currentParent)
        {
          if (m_tokens[i].type != eWhiteSpace)
          {
            throw csParseException("Found non white space before the first tag",
                                   m_tokens[i].line,
                                   m_tokens[i].column);
          }
        }
        else
        {
          std::string text;
          while (true)
          {
            if (i >= m_tokens.size())
            {
              throw csParseException("Tag '" + currentParent->GetTagName() + "' not closed. ", 0, 0);
            }

            if (m_tokens[i].type == eAngleBracketOpen)
            {
              currentParent->CreateChildText(text, true);
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
  }
  catch (const csParseException &e)
  {
    delete document;
    throw e;
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
  size_t line   = 0;
  size_t column = 0;
  m_tokens.clear();
  while (!IsEOF())
  {
    size_t nextLine   = line;
    size_t nextColumn = column + 1;
    char   ch         = Pick();
    if (ch == '=')
    {
      m_tokens.emplace_back(eEquals, std::string(1, ch), line, column);
    }
    else if (ch == '<')
    {
      m_tokens.emplace_back(eAngleBracketOpen, std::string(1, ch), line, column);
    }
    else if (ch == '>')
    {
      m_tokens.emplace_back(eAngleBracketClose, std::string(1, ch), line, column);
    }
    else if (ch == '/')
    {
      m_tokens.emplace_back(eSlash, std::string(1, ch), line, column);
    }
    else if (ch == '!')
    {
      m_tokens.emplace_back(eExclamationMark, std::string(1, ch), line, column);
    }
    else if (ch == '-')
    {
      m_tokens.emplace_back(eDash, std::string(1, ch), line, column);
    }
    else if (ch == '[')
    {
      m_tokens.emplace_back(eSquareBracketsOpen, std::string(1, ch), line, column);
    }
    else if (ch == ']')
    {
      m_tokens.emplace_back(eSquareBracketsClose, std::string(1, ch), line, column);
    }
    else
    {
      Put();
      nextColumn--;
      ReadOtherToken(line, column, nextLine, nextColumn);
    }

    line = nextLine;
    column = nextColumn;
  }
}

static void update_column_line (char ch, bool &pendingNewLine, bool &pendingReturn, size_t  &nextLine, size_t &nextColumn)
{
  if (ch == '\n')
  {
    if (pendingReturn)
    {
      pendingReturn = false;
      return;
    }
    nextLine++;
    nextColumn = 0;
    pendingNewLine = true;
  }
  else if (ch == '\r')
  {
    if (pendingNewLine)
    {
      pendingNewLine = false;
      return;
    }
    nextLine++;
    nextColumn = 0;
    pendingReturn = true;
  }
  else
  {
    pendingReturn = false;
    pendingNewLine = false;
    nextColumn++;
  }
}


void csParser::ReadOtherToken(size_t line, size_t column, size_t &nextLine, size_t &nextColumn)
{
  bool pendingNewLine = false;
  bool pendingReturn = false;


  char ch = Pick();
  if (ch == '"')
  {
    update_column_line(ch, pendingNewLine, pendingReturn, nextLine, nextColumn);
    std::string content;
    while (true)
    {
      if (IsEOF())
      {
        return;
      }

      ch = Pick();
      update_column_line(ch, pendingNewLine, pendingReturn, nextLine, nextColumn);
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
        update_column_line(ch, pendingNewLine, pendingReturn, nextLine, nextColumn);
      }
      content += ch;

    }

    m_tokens.emplace_back(eString, content, line, column);
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
      update_column_line(ch, pendingNewLine, pendingReturn, nextLine, nextColumn);
      content += ch;
    }

    m_tokens.emplace_back(eIdent, content, line, column);
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
      update_column_line(ch, pendingNewLine, pendingReturn, nextLine, nextColumn);
      content += ch;
    }

    m_tokens.emplace_back(eWhiteSpace, content, line, column);
  }
  else
  {
    if (ch != 0)
    {
      update_column_line(ch, pendingNewLine, pendingReturn, nextLine, nextColumn);
      m_tokens.emplace_back(eOther, std::string(1, ch), line, column);
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

bool csNode::IsElement() const
{
  return m_type == eNT_Element;
}


bool csNode::IsText() const
{
  return m_type == eNT_Text;
}


bool csNode::IsComment() const
{
  return m_type == eNT_Comment;
}

csElement* csNode::AsElement()
{
  return nullptr;
}

const csElement* csNode::AsElement() const
{
  return nullptr;
}

csText* csNode::AsText()
{
  return nullptr;
}

const csText* csNode::AsText() const
{
  return nullptr;
}

csComment* csNode::AsComment()
{
  return nullptr;
}

const csComment* csNode::AsComment() const
{
  return nullptr;
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

csElement* csElement::AsElement ()
{
  return this;
}

const csElement* csElement::AsElement () const
{
  return this;
}


csElement *csElement::CreateChildElement(const std::string &tagName)
{
  auto child = new csElement(tagName);
  child->m_parent = this;
  m_children.push_back(child);
  return child;
}

inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

void csElement::CreateChildText(const std::string &text, bool trim)
{
  std::string copy = text;
  if (trim)
  {
    ltrim(copy);
    rtrim(copy);
  }

  if (copy.empty())
  {
    return;
  }
  auto child = new csText();
  child->SetContent(copy);
  child->m_parent = this;
  m_children.push_back(child);
}


void csElement::CreateChildComment(const std::string &text)
{
  auto child = new csComment();
  child->SetContent(text);
  child->m_parent = this;
  m_children.push_back(child);
}

size_t csElement::GetNumberOfChildren() const
{
  return m_children.size();
}

csNode* csElement::GetChild(size_t idx)
{
  return m_children[idx];
}

const csNode* csElement::GetChild(size_t idx) const
{
  return m_children[idx];
}


bool csElement::HasAttribute(const std::string &key) const
{
  for (const auto &attrib: m_attributes)
  {
    if (attrib->GetKey() == key)
    {
      return true;
    }
  }
  return false;
}

std::string csElement::GetAttribute(const std::string &key) const
{
  for (const auto &attrib: m_attributes)
  {
    if (attrib->GetKey() == key)
    {
      return attrib->GetValue();
    }
  }
  return "";
}

std::string csElement::GetContent () const
{
  std::string content;
  for (const auto &child: m_children)
  {
    const xml::csText *text = child->AsText();
    if (text)
    {
      content += text->GetContent();
    }

  }
  return content;
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


csText* csText::AsText ()
{
  return this;
}

const csText* csText::AsText () const
{
  return this;
}

csComment::csComment()
    : csNode(eNT_Comment)
{

}

void csComment::SetContent(const std::string &content)
{
  m_content = content;
}

const std::string &csComment::GetContent() const
{
  return m_content;
}

csComment* csComment::AsComment ()
{
  return this;
}

const csComment* csComment::AsComment () const
{
  return this;
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