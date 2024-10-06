#include <csXml/csXml.hh>
#include <cstdio>
#include <vector>

namespace cryo::xml
{


csDocument csParser::ParseFilename(const std::string &filename)
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

  char *buffer = new char [size+1];
  memset(buffer, 0, size+1);

  fread(buffer, sizeof(char), size, fs);
  fclose(fs);

  std::string content (buffer, size);
  delete[] buffer;

  return ParseContent(content);
}

csDocument csParser::ParseContent(const std::string &content)
{
  csParser parser(content);
  return parser.Parse();
}

csParser::csParser(const std::string &content)
: m_content(content)
, m_idx(0)
{
}

csDocument csParser::Parse()
{
  Tokenize();


  return {};
}



void csParser::Tokenize()
{
  m_tokens.clear();
  while(!IsEOF())
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

    m_tokens.emplace_back(eWhiteSpace, content);  }
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
  if (m_idx>=1)
  {
    m_idx--;
  }
}



}