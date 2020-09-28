

#pragma once


namespace Spice::moc
{


class ClassNode;
class ClassSuperDefinition;
class NamespaceNode;
class BlockNode;
class ASTNode;
class CSMetaNode;
class VisibilityNode;
class FunctionNode;
class MemberNode;
class Token;
class TypeDef;
class Tokenizer;

class Parser
{
public:
  Parser();

  ASTNode* Parse(Tokenizer& tokenizer);

private:
  BlockNode* ParseBlock(Tokenizer& tokenizer, size_t& idx, ASTNode* parent);
  ASTNode* ParseNode(Tokenizer& tokenizer, Token& token, size_t& idx, ASTNode* parent);
  NamespaceNode* ParseNamespace(Tokenizer& tokenizer, size_t& idx, ASTNode* parent);
  ClassNode* ParseClass(Tokenizer& tokenizer, size_t& idx, ASTNode* parent);
  VisibilityNode* ParseVisibility(Tokenizer& tokenizer, size_t& idx, ASTNode* parent);
  ClassSuperDefinition ParseSuperDefinition(Tokenizer& tokenizer, size_t& idx, ASTNode* parent);
  ASTNode* ParseFunctionOrMember(Tokenizer& tokenizer, size_t& idx, ASTNode* parent);
  FunctionNode* ParseFunction(Tokenizer& tokenizer, size_t& idx, ASTNode* parent);
  MemberNode* ParseMember(Tokenizer& tokenizer, size_t& idx, ASTNode* parent);
  CSMetaNode* ParseCSMeta(Tokenizer& tokenizer, size_t& idx, ASTNode* parent);

  void SkipEnum(Tokenizer& tokenizer, size_t& idx);
  void SkipBlock(Tokenizer& tokenizer, size_t& idx);
  void SkipArrayInitializer(Tokenizer& tokenizer, size_t& idx);


  std::string ReverseName(Tokenizer& tokenizer, size_t& idx);

  TypeDef ReverseType(Tokenizer& tokenizer, size_t& idx);

  TypeDef GetType(Tokenizer& tokenizer, size_t& idx);
  std::string GetName(Tokenizer& tokenizer, size_t& idx);

};


}
