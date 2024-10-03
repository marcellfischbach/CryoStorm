#pragma once


#include <string>
#include <stdint.h>

namespace cryo::moc
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
  Token(const std::string& token = "", uint32_t line = 0, uint32_t column = 0);

  const std::string& Get() const
  {
    return m_token;
  }

  TokenType GetType() const
  {
    return m_type;
  }

  uint32_t getLine() const
  {
    return m_line;
  }
  uint32_t getColumn() const
  {
    return m_column;
  }

private:
  bool IsIdentifier();
  
  TokenType m_type;
  std::string m_token;

  uint32_t m_line;
  uint32_t m_column;

};


}
