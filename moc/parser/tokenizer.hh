
#pragma once

#include <parser/token.hh>
#include <string>
#include <vector>

namespace cs::moc
{


class SourceFile;
class Tokenizer
{
public:
  Tokenizer(const SourceFile& sourceFile);

  const std::vector<Token>& GetTokens() const;

  size_t Find(TokenType type, size_t offset) const;
  size_t FindBack(TokenType type, size_t offset) const;
  bool Has(TokenType type, size_t startIncl, size_t endIncl) const;

private:
  bool IsIdentifier(const std::string& token) const;
  
  std::vector<Token> m_tokens;
};


}