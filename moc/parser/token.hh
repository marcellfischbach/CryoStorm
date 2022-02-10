#pragma once


#include <string>

namespace ce::moc
{

enum TokenType
{
  eTT_Identifier,
  eTT_Class,
  eTT_Struct,
  eTT_Namespace,
  eTT_Private,
  eTT_Protected,
  eTT_Public,
  eTT_Virtual,
  eTT_Const,
  eTT_CurlyBraceOpen,
  eTT_CurlyBraceClose,
  eTT_ParenOpen,
  eTT_ParenClose,
  eTT_AngleBracketOpen,
  eTT_AngleBracketClose,
  eTT_BracketOpen,
  eTT_BracketClose,
  eTT_Colon,
  eTT_DoubleColon,
  eTT_SemiColon,
  eTT_Comma,
  eTT_Dot,
  eTT_DotDotDot,
  eTT_Ampersand,
  eTT_Asterisk,
  eTT_DoubleAsterisk,
  eTT_Equal,
  eTT_Tilde,
  eTT_Enum,
  eTT_Template,
  eTT_Friend,
  eTT_CS_CLASS,
  eTT_CS_PROPERTY,
  eTT_CS_FUNCTION,
  eTT_OtherCode,
};

class Token
{
public:
  Token(const std::string& token = "");

  const std::string& Get() const
  {
    return m_token;
  }

  TokenType GetType() const
  {
    return m_type;
  }


private:
  bool IsIdentifier();
  
  TokenType m_type;
  std::string m_token;
};


}
