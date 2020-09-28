
#include <parser/tokenizer.hh>
#include <parser/sourcefile.hh>
#include <parser/parseexception.hh>

#define PUSH(tkn) if (!tkn.empty()) { m_tokens.push_back(Token(tkn)); } tkn = ""
#define PUSH_CH(tkn, ch) PUSH(tkn); tkn += ch; PUSH(tkn)


namespace Spice::moc
{
Tokenizer::Tokenizer(const SourceFile& sourceFile)
{
  SourceFileIterator it = sourceFile.GetIterator();
  std::string tkn = "";
  char ch = ' ';

  bool singleChar = false;
  bool string = false;

  while (it.HasMore())
  {
    ch = it.Next();

    if (string)
    {
      tkn += ch;
      if (ch == '\\')
      {
        if (it.HasMore())
        {
          ch = it.Next();
          tkn += ch;
        }
      }
      else if (ch == '"')
      {
        string = false;
        PUSH(tkn);
      }
    }
    else if (singleChar)
    {
      tkn += ch;
      if (ch == '\\')
      {
        if (it.HasMore())
        {
          ch = it.Next();
          tkn += ch;
        }
      }
      else if (ch == '\'')
      {
        singleChar = false;
        PUSH(tkn);
      }
    }
    else
    {


      switch (ch)
      {
      case '"':
        PUSH(tkn);
        tkn += ch;
        string = true;
        break;
      case '\'':
        PUSH(tkn);
        tkn += ch;
        singleChar = true;
        break;

      case '{':
      case '}':
      case '[':
      case ']':
      case '(':
      case ')':
      case ';':
      case ',':
      case '<':
      case '+':
      case '-':
      case '=':
      case '/':
      case '~':
        PUSH(tkn);
        tkn += ch;
        break;

      case '*':
        if (tkn != "*")
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;

      case '&':
        if (tkn != "&")
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;

      case '|':
        if (tkn != "|")
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;

      case '>':
        if (tkn != "-")
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;


      case ':':
        if (tkn != ":")
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;
      case '.':
        if (tkn != "." && tkn != "..")
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;
      case ' ':
      case '\t':
        PUSH(tkn);
        break;

      default:
        if (!IsIdentifier(tkn))
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;
      }
    }
  }
  PUSH(tkn);
}


bool Tokenizer::IsIdentifier(const std::string& token) const
{

  if (token.size() == 0)
  {
    return false;
  }

  char ch = token[0];
  if (!(::isalpha(ch) || ch == '_'))
  {
    return false;
  }


  for (size_t i = 1, in = token.size(); i < in; ++i)
  {
    ch = token[0];
    if (!(::isalnum(ch) || ch == '_'))
    {
      return false;
    }
  }
  return true;
}


const std::vector<Token> &Tokenizer::GetTokens() const
{
  return m_tokens;
}

size_t Tokenizer::Find(TokenType type, size_t offset) const
{
  for (size_t i = offset, in = m_tokens.size(); i < in; ++i)
  {
    if (m_tokens[i].GetType() == type)
    {
      return i;
    }
  }

  throw ParseException(__FILE__, __LINE__);
}


size_t Tokenizer::FindBack(TokenType type, size_t offset) const
{
  for (size_t i = offset; i >= 0; --i)
  {
    if (m_tokens[i].GetType() == type)
    {
      return i;
    }
    if (i == 0)
    {
      throw ParseException(__FILE__, __LINE__);
    }
  }

  throw ParseException(__FILE__, __LINE__);
}



bool Tokenizer::Has(TokenType type, size_t startIncl, size_t endIncl) const
{
  for (size_t i = startIncl, in = m_tokens.size(); i < in && i <= endIncl; ++i)
  {
    if (m_tokens[i].GetType() == type)
    {
      return true;
    }
  }
  return false;
}



}
