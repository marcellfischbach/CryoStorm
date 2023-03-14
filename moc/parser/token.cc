

#include <parser/token.hh>


namespace ce::moc
{

#define CHECK_TKN(tkn, tknEnum) if (m_token == tkn)  m_type = tknEnum
#define E_CHECK_TKN(tkn, tknEnum) else if (m_token == tkn)  m_type = tknEnum

Token::Token(const std::string & token, uint32_t line, uint32_t column)
  : m_token(token)
  , m_line(line)
  , m_column(column)
{
  CHECK_TKN("class", eTT_Class);
  E_CHECK_TKN("struct", eTT_Struct);
  E_CHECK_TKN("namespace", eTT_Namespace);
  E_CHECK_TKN("private", eTT_Private);
  E_CHECK_TKN("protected", eTT_Protected);
  E_CHECK_TKN("public", eTT_Public);
  E_CHECK_TKN("virtual", eTT_Virtual);
  E_CHECK_TKN("const", eTT_Const);
  E_CHECK_TKN("{", eTT_CurlyBraceOpen);
  E_CHECK_TKN("}", eTT_CurlyBraceClose);
  E_CHECK_TKN("<", eTT_AngleBracketOpen);
  E_CHECK_TKN(">", eTT_AngleBracketClose);
  E_CHECK_TKN("(", eTT_ParenOpen);
  E_CHECK_TKN(")", eTT_ParenClose);
  E_CHECK_TKN("[", eTT_BracketOpen);
  E_CHECK_TKN("]", eTT_BracketClose);
  E_CHECK_TKN(":", eTT_Colon);
  E_CHECK_TKN("::", eTT_DoubleColon);
  E_CHECK_TKN(";", eTT_SemiColon);
  E_CHECK_TKN(",", eTT_Comma);
  E_CHECK_TKN(".", eTT_Dot);
  E_CHECK_TKN("...", eTT_DotDotDot);
  E_CHECK_TKN("=", eTT_Equal);
  E_CHECK_TKN("~", eTT_Tilde);
  E_CHECK_TKN("&", eTT_Ampersand);
  E_CHECK_TKN("*", eTT_Asterisk);
  E_CHECK_TKN("**", eTT_DoubleAsterisk);
  E_CHECK_TKN("enum", eTT_Enum);
  E_CHECK_TKN("template", eTT_Template);
  E_CHECK_TKN("friend", eTT_Friend);
  E_CHECK_TKN("CE_CLASS", eTT_CS_CLASS);
  E_CHECK_TKN("CE_PROPERTY", eTT_CS_PROPERTY);
  E_CHECK_TKN("CE_FUNCTION", eTT_CS_FUNCTION);
else if (IsIdentifier())
  {
    m_type = eTT_Identifier;
  }
  else
  {
    m_type = eTT_OtherCode;
  }

}

bool Token::IsIdentifier()
{

  if (m_token.size() == 0)
  {
    return false;
  }

  char ch = m_token[0];
  if (!(::isalpha(ch) || ch == '_'))
  {
    return false;
  }


  for (size_t i = 1, in = m_token.size(); i < in; ++i)
  {
    ch = m_token[0];
    if (!(::isalnum(ch) || ch == '_'))
    {
      return false;
    }
  }
  return true;
}


}
