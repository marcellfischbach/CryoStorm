
#pragma once

#include <csXml/csXmlExport.hh>
#include <string>
#include <cstdint>
#include <vector>

namespace cryo::xml
{

class CS_XML_API csDocument
{

};




class CS_XML_API csParser
{
public:
  static csDocument ParseFilename(const std::string &filename);
  static csDocument ParseContent(const std::string &content);

private:
  explicit csParser(const std::string &content);
  csDocument Parse ();
  void ReadOtherToken ();

  char Pick();
  void Put();

  void Tokenize ();
  bool IsEOF() const;

private:

  enum eTokenType
  {
    eIdent,
    eString,
    eWhiteSpace,
    eEquals,
    eAngleBracketOpen,
    eAngleBracketClose,
    eSlash,
    eExclamationMark,
    eSquareBracketsOpen,
    eSquareBracketsClose,
    eOther
  };

  struct Token
  {
    eTokenType type;
    std::string content;

  };

  std::string m_content;
  uint32_t  m_idx;
  std::vector<Token> m_tokens;
};


}
