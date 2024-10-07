
#pragma once

#include <csXml/csXmlExport.hh>

#include <cstdint>
#include <exception>
#include <string>
#include <vector>


namespace cryo::xml
{

class CS_XML_API csAttribute
{
public:
  explicit csAttribute(std::string key);

  [[nodiscard]] const std::string &GetKey() const;

  void SetValue(const std::string &value);
  [[nodiscard]] const std::string &GetValue() const;
private:
  std::string m_key;
  std::string m_value;
};


enum eNodeType
{
  eNT_Element,
  eNT_Text,
  eNT_Comment
};

class csElement;
class CS_XML_API csNode
{
  friend class csDocument;
  friend class csElement;
public:
  virtual ~csNode() = default;

  [[nodiscard]] eNodeType GetType() const;


  [[nodiscard]] const csElement *GetParent() const;
  [[nodiscard]] csElement *GetParent();

protected:
  explicit csNode(eNodeType type);

protected:
  csElement *m_parent;

private:
  eNodeType m_type;


};

class csText;
class csComment;
class CS_XML_API csElement : public csNode
{
  friend class csDocument;
public:
  ~csElement() override;

  [[nodiscard]] const std::string &GetTagName() const;

  csElement *CreateChildElement(const std::string &tagName);
  void CreateChildText(const std::string &text, bool trim);
  void CreateChildComment(const std::string &text);


  void AddAttribute(const std::string &key, const std::string &value);


private:
  explicit csElement(std::string tagName);

private:
  std::string           m_tagName;
  std::vector<csNode *> m_children;

  std::vector<csAttribute *> m_attributes;

};

class CS_XML_API csText : public csNode
{
public:
  csText();
  ~csText() override = default;

  void SetContent(const std::string &content);
  [[nodiscard]] const std::string &GetContent() const;
private:
private:
  std::string m_content;
};

class CS_XML_API csComment : public csNode
{
public:
  csComment();
  ~csComment() override = default;

  void SetContent(const std::string &content);
  [[nodiscard]] const std::string &GetContent() const;
private:
private:
  std::string m_content;
};



class CS_XML_API csParseException : public std::exception
{
public:
  csParseException(const std::string &message, size_t line, size_t column);
  size_t GetLine () const;
  size_t GetColumn () const;

private:
  size_t m_line;
  size_t m_column;
};

class CS_XML_API csDocument
{
  friend class csParser;
public:
  ~csDocument();

  csElement *CreateRoot(const std::string &tagName);


  [[nodiscard]] const csElement *GetRoot() const;

private:
  csDocument() = default;

  csElement *m_root = nullptr;
};


class CS_XML_API csParser
{
public:
  static csDocument *ParseFilename(const std::string &filename);
  static csDocument *ParseContent(const std::string &content);

private:
  explicit csParser(const std::string &content);
  csDocument *Parse();

  void ReadOtherToken(size_t line, size_t column, size_t &nextLine, size_t &nextColumn);

  char Pick();
  void Put();

  void Tokenize();
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
    eDash,
    eSquareBracketsOpen,
    eSquareBracketsClose,
    eOther
  };

  struct Token
  {
    eTokenType  type;
    std::string content;
    size_t      line;
    size_t      column;
  };

  std::string ReadFullIdent(size_t &tokenIdx);

  std::string        m_content;
  uint32_t           m_idx;
  std::vector<Token> m_tokens;

};


}
