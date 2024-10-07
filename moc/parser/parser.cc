#include <parser/parseexception.hh>
#include <parser/parser.hh>
#include <ast.hh>
#include <parser/tokenizer.hh>
#include <deque>
#include <list>

#define CHECK_IDX(tokens, idx) if ((idx) >= tokens.size()) throw cryo::moc::ParseException(__FILE__, __LINE__, "Index out of bounds " + std::to_string(idx) + " >= " + std::to_string(tokens.size()), 0, 0)

namespace cryo::moc
{

Parser::Parser()
{


}


ASTNode *Parser::Parse(Tokenizer &tokenizer)
{
  size_t idx = 0;
  return ParseBlock(tokenizer, idx, nullptr);
}

BlockNode *Parser::ParseBlock(Tokenizer &tokenizer, size_t &idx, ASTNode *parent)
{
  BlockNode   *blockNode = new BlockNode();
  auto        tokens     = tokenizer.GetTokens();
  uint32_t    line       = 0;
  uint32_t    column     = 0;
  bool        first      = true;
  for (size_t in         = tokens.size(); idx < in;)
  {
    CHECK_IDX(tokens, idx);
    Token &token = tokens[idx];
    if (first)
    {
      line   = token.getLine();
      column = token.getColumn();
      first  = false;
    }
    if (token.GetType() == eTT_CurlyBraceClose)
    {
      ++idx;
      if (idx < in && tokens[idx].GetType() == eTT_SemiColon)
      {
        ++idx;
      }
      break;
    }

    size_t  IDX   = idx;
    ASTNode *node = ParseNode(tokenizer, token, idx, blockNode);
    if (node)
    {
      blockNode->Add(node);
    }
    if (IDX == idx)
    {
      throw ParseException(__FILE__, __LINE__, "Unable to find end of block", line, column);
    }
  }
  return blockNode;
}


ASTNode *Parser::ParseNode(Tokenizer &tokenizer, Token &token, size_t &idx, ASTNode *parent)
{
  switch (token.GetType())
  {
    case eTT_CurlyBraceOpen:
      return ParseBlock(tokenizer, ++idx, parent);
    case eTT_Namespace:
      return ParseNamespace(tokenizer, idx, parent);
    case eTT_Class:
    case eTT_Struct:
      return ParseClass(tokenizer, idx, parent);
    case eTT_Template:
      SkipTemplate(tokenizer, idx);
      return nullptr;

    case eTT_CS_CLASS:
    case eTT_CS_PROPERTY:
    case eTT_CS_FUNCTION:
      return ParseCSMeta(tokenizer, idx, parent);

    case eTT_Enum:
      SkipEnum(tokenizer, idx);
      break;

    case eTT_Public:
    case eTT_Private:
    case eTT_Protected:
      return ParseVisibility(tokenizer, idx, parent);


    default:
      return ParseFunctionOrMember(tokenizer, idx, parent);

      //    idx++;
      //    return  nullptr;

  }
  return nullptr;

}


NamespaceNode *Parser::ParseNamespace(Tokenizer &tokenizer, size_t &idx, ASTNode *parent)
{
  NamespaceNode *namespaceNode = new NamespaceNode();
  std::string   name           = "";
  std::string   alias          = "";
  auto          tokens         = tokenizer.GetTokens();
  BlockNode     *blockNode;
  bool          namespaceDone  = false;
  for (size_t   i              = idx + 1, in = tokens.size(); i < in && !namespaceDone; ++i)
  {
    CHECK_IDX(tokens, i);
    Token &token = tokens[i];
    switch (token.GetType())
    {
      case eTT_CurlyBraceOpen:
        blockNode = ParseBlock(tokenizer, ++i, namespaceNode);
        if (blockNode)
        {
          namespaceNode->Add(blockNode);
        }
        namespaceDone = true;
        // when read block returns the index (i) already points to the next token
        idx           = i;
        break;
      case eTT_SemiColon:
        // when ending with a semicolon in index must be pointing to the next token (i+1)
        namespaceDone = true;
        idx           = i + 1;
        break;
      case eTT_Equal:
        alias = name;
        name  = "";
        break;
      case eTT_Identifier:
      case eTT_DoubleColon:
        name += token.Get();
        break;
    }

  }
  namespaceNode->SetName(name);
  namespaceNode->SetAlias(alias);

  return namespaceNode;
}

ClassNode *Parser::ParseClass(Tokenizer &tokenizer, size_t &idx, ASTNode *parent)
{
  ClassNode *classNode = new ClassNode();

  std::string name   = "";
  std::string alias  = "";
  auto        tokens = tokenizer.GetTokens();
  CHECK_IDX(tokens, idx);
  Token &token = tokens[idx];
  classNode->SetStruct(token.GetType() == eTT_Struct);
  idx++;
  for (size_t in = tokens.size(); idx < in;)
  {
    CHECK_IDX(tokens, idx);
    Token &token = tokens[idx];
    if (token.GetType() == eTT_Colon)
    {
      CHECK_IDX(tokens, idx - 1);
      classNode->SetName(tokens[idx - 1].Get());
      idx++;
      ClassSuperDefinition super = ParseSuperDefinition(tokenizer, idx, classNode);
      classNode->AddSuper(super);
    }
    else if (token.GetType() == eTT_Comma)
    {
      idx++;
      ClassSuperDefinition super = ParseSuperDefinition(tokenizer, idx, classNode);
      classNode->AddSuper(super);
    }
    else if (token.GetType() == eTT_CurlyBraceOpen)
    {
      if (classNode->GetName().empty())
      {
        CHECK_IDX(tokens, idx - 1);
        classNode->SetName(tokens[idx - 1].Get());
      }
      idx++;
      BlockNode *block = ParseBlock(tokenizer, idx, classNode);
      classNode->Add(block);
      if (idx < tokens.size() && tokens[idx].GetType() == eTT_SemiColon)
      {
        idx++;
      }
      break;
    }
    else if (token.GetType() == eTT_SemiColon)
    {
      if (classNode->GetName().empty())
      {
        CHECK_IDX(tokens, idx - 1);
        classNode->SetName(tokens[idx - 1].Get());
      }
      idx++;
      break;
    }
    else
    {
      idx++;
    }
  }

  return classNode;
}


ClassSuperDefinition Parser::ParseSuperDefinition(Tokenizer &tokenizer, size_t &idx, ASTNode *parent)
{
  auto tokens = tokenizer.GetTokens();


  TypeDef     type;
  std::string visibility = "";
  bool        virtuality = false;
  bool        csSuper    = false;

  uint32_t line   = 0;
  uint32_t column = 0;
  bool     first  = true;

  for (; idx < tokens.size();)
  {
    CHECK_IDX(tokens, idx);
    Token &token = tokens[idx];
    if (first)
    {
      line   = token.getLine();
      column = token.getColumn();
      first  = false;
    }
    switch (token.GetType())
    {
      case eTT_Public:
      case eTT_Protected:
      case eTT_Private:
        visibility = token.Get();
        idx++;
        break;
      case eTT_Virtual:
        virtuality = true;
        idx++;
        break;

      case eTT_Comma:
      case eTT_CurlyBraceOpen:
        return ClassSuperDefinition(csSuper, type, visibility, virtuality);

      case eTT_Identifier:
        CHECK_IDX(tokens, idx + 1);
        if (token.Get() == std::string("CS_SUPER") && idx + 1 < tokens.size() &&
            tokens[idx + 1].GetType() == eTT_ParenOpen)
        {
          csSuper = true;
          idx += 2;
        }
        else
        {
          type = GetType(tokenizer, idx);
        }
        break;
      case eTT_ParenClose:
        if (!csSuper)
        {
          throw ParseException(__FILE__, __LINE__, "Unexpected token ')'", token.getLine(), token.getColumn());
        }
        idx++;
        break;
      default:
        printf("Unexpected token [\"%s\"][%d]\n",
               token.Get().c_str(),
               token.GetType(),
               token.getLine(),
               token.getColumn());
        throw ParseException(__FILE__,
                             __LINE__,
                             "Unexpected token [" + token.Get() + "]",
                             token.getLine(),
                             token.getColumn());
    }
  }

  throw ParseException(__FILE__, __LINE__, "Cannot read super type.", line, column);
}


VisibilityNode *Parser::ParseVisibility(Tokenizer &tokenizer, size_t &idx, ASTNode *parent)
{
  const Token &token = tokenizer.GetTokens()[idx + 1];
  if (token.GetType() != eTT_Colon)
  {
    throw ParseException(__FILE__, __LINE__, "Unable to parse visibility", token.getLine(), token.getColumn());
  }
  idx += 2;
  return new VisibilityNode(tokenizer.GetTokens()[idx - 2].Get());
}


ASTNode *Parser::ParseFunctionOrMember(Tokenizer &tokenizer, size_t &idx, ASTNode *parent)
{

  auto        tokens = tokenizer.GetTokens();
  for (size_t i      = idx, in = tokens.size(); i < in; ++i)
  {
    CHECK_IDX(tokens, i);
    Token &token = tokens[i];
    if (token.GetType() == eTT_ParenOpen)
    {
      return ParseFunction(tokenizer, idx, parent);
    }
    else if (token.GetType() == eTT_SemiColon)
    {
      return ParseMember(tokenizer, idx, parent);
    }
    else if (token.GetType() == eTT_Enum)
    {
      size_t idxCB = tokenizer.Find(eTT_CurlyBraceOpen, i);
      SkipBlock(tokenizer, idxCB);
      idx = idxCB;
      return nullptr;
    }
    else if (token.GetType() == eTT_Class
             || token.GetType() == eTT_Struct
             || token.GetType() == eTT_Public
             || token.GetType() == eTT_Private
             || token.GetType() == eTT_Protected
        )
    {
      break;
    }
    else if (token.GetType() == eTT_CurlyBraceOpen)
    {
      size_t idxCB = tokenizer.Find(eTT_CurlyBraceClose, i);
      idx = idxCB + 1;
      return nullptr;

    }
  }


  idx++;
  CHECK_IDX(tokens, idx - 1);
  return new TokenNode(tokens[idx - 1]);
}

FunctionNode *Parser::ParseFunction(Tokenizer &tokenizer, size_t &idx, ASTNode *parent)
{
  auto tokens = tokenizer.GetTokens();

  size_t      idxParen = tokenizer.Find(eTT_ParenOpen, idx);
  size_t      lIDX     = idxParen;
  std::string name     = ReverseName(tokenizer, lIDX);
  TypeDef     def;
  if (lIDX >= idx)
  {
    CHECK_IDX(tokens, lIDX);
    if (tokens[lIDX].GetType() == eTT_Tilde)
    {
      name = "~" + name;
      lIDX--;
    }
  }

  if (lIDX >= idx)
  {
    size_t defIdx = idx;
    def = GetType(tokenizer, defIdx);
  }

  FunctionNode *func = new FunctionNode();
  func->SetName(name);
  func->SetReturnValue(def);

  try
  {
    size_t virtualIdx = tokenizer.Find(eTT_Virtual, idx);
    if (virtualIdx < idxParen)
    {
      func->SetVirtual(true);
    }
  }
  catch (std::exception &)
  {
    //
  }

  idx = idxParen + 1;
  for (size_t in = tokens.size(); idx < in;)
  {
    CHECK_IDX(tokens, idx);
    Token &token = tokens[idx];
    if (token.GetType() == eTT_ParenClose)
    {
      break;
    }
    if (token.GetType() == eTT_Comma)
    {
      idx++;
      continue;
    }
    TypeDef def = GetType(tokenizer, idx);

    CHECK_IDX(tokens, idx);
    token = tokens[idx];
    if (token.GetType() == eTT_Comma || token.GetType() == eTT_ParenClose)
    {
      func->Add(Argument(def));
    }

    if (token.GetType() == eTT_Identifier)
    {
      std::string name = GetName(tokenizer, idx);
      CHECK_IDX(tokens, idx);
      bool defaultValue = tokens[idx].GetType() == eTT_Equal;
      func->Add(Argument(def, name, defaultValue));
    }

    CHECK_IDX(tokens, idx);
    token = tokens[idx];
    if (token.GetType() == eTT_Comma)
    {
      idx++;
    }
    else if (token.GetType() == eTT_ParenClose)
    {
      break;
    }
    else
    {
      int         paren          = 0;
      int         angularBracket = 0;
      int         curlyBrace     = 0;
      int         bracket        = 0;
      for (size_t in             = tokens.size(); idx < in; idx++)
      {
        CHECK_IDX(tokens, idx);
        Token &token = tokens[idx];
        switch (token.GetType())
        {
          case eTT_ParenOpen:
            paren++;
            break;
          case eTT_ParenClose:
            if (paren > 0)
            {
              paren--;
            }
            break;
          case eTT_AngleBracketOpen:
            angularBracket++;
            break;
          case eTT_AngleBracketClose:
            angularBracket--;
            break;
          case eTT_CurlyBraceOpen:
            curlyBrace++;
            break;
          case eTT_CurlyBraceClose:
            curlyBrace--;
            break;
        }
        if (token.GetType() == eTT_Comma || token.GetType() == eTT_ParenClose)
        {
          if (paren == 0 && angularBracket == 0 && curlyBrace == 0 && bracket == 0)
          {
            break;
          }
        }
      }
    }
  }

  for (size_t in = tokens.size(); idx < in;)
  {
    CHECK_IDX(tokens, idx);
    Token &token = tokens[idx++];
    if (token.GetType() == eTT_SemiColon)
    {
      break;
    }
    else if (token.GetType() == eTT_CurlyBraceOpen)
    {
      SkipBlock(tokenizer, idx);
      break;
    }
    else if (token.GetType() == eTT_Const)
    {
      func->SetConst(true);
    }
    else if (token.GetType() == eTT_Equal)
    {
      CHECK_IDX(tokens, idx);
      if (tokens[idx].Get() == "0")
      {
        func->SetPureVirtual(true);
      }
    }

  }


  return func;
}


MemberNode *Parser::ParseMember(Tokenizer &tokenizer, size_t &idx, ASTNode *parent)
{
  auto   tokens       = tokenizer.GetTokens();
  size_t startIdx     = idx;
  size_t semiColonIdx = tokenizer.Find(eTT_SemiColon, idx);
  size_t typeIdx      = semiColonIdx;
  try
  {
    size_t equalIdx = tokenizer.Find(eTT_Equal, idx);
    if (equalIdx < typeIdx)
    {
      typeIdx = equalIdx;
    }

  }
  catch (std::exception &)
  {

  }

  SkipArrayInitializer(tokenizer, typeIdx);
  typeIdx++;

  std::string name = ReverseName(tokenizer, typeIdx);
  typeIdx = idx;
  TypeDef type = GetType(tokenizer, typeIdx);

  idx = semiColonIdx + 1;
  MemberNode *memberNode = new MemberNode();
  memberNode->SetName(name);
  memberNode->SetType(type);
  return memberNode;
}

void Parser::SkipArrayInitializer(Tokenizer &tokenizer, size_t &idx)
{
  auto     tokens = tokenizer.GetTokens();
  uint32_t line   = 0;
  uint32_t column = 0;
  if (idx < tokens.size())
  {
    const Token &token = tokens[idx];
    line   = token.getLine();
    column = token.getColumn();
  }
  for (; idx >= 0;)
  {
    if (idx == 0)
    {
      throw ParseException(__FILE__, __LINE__, "Unable to skip array initializer", line, column);
    }
    CHECK_IDX(tokens, idx - 1);
    Token &token = tokens[--idx];
    if (token.GetType() != eTT_BracketClose)
    {
      return;
    }
    idx          = tokenizer.FindBack(eTT_BracketOpen, idx);
  }
}

void Parser::SkipTemplate(Tokenizer &tokenizer, size_t &idx)
{
  const std::vector<Token> &tokens    = tokenizer.GetTokens();
  int                      numAngular = 0;
  while (idx < tokens.size())
  {
    CHECK_IDX(tokens, idx);
    const Token &token = tokens[idx++];
    if (token.GetType() == eTT_AngleBracketOpen)
    {
      numAngular++;
    }
    else if (token.GetType() == eTT_AngleBracketClose)
    {
      numAngular--;
      if (numAngular == 0)
      {
        break;
      }
    }
  }
}

void Parser::SkipEnum(Tokenizer &tokenizer, size_t &idx)
{
  auto        tokens = tokenizer.GetTokens();
  uint32_t    line   = 0;
  uint32_t    column = 0;
  bool        first  = true;
  for (size_t i      = idx, in = tokens.size(); i < in; ++i)
  {
    Token &token = tokens[i];
    if (first)
    {
      line   = token.getLine();
      column = token.getColumn();
      first  = false;
    }
    if (token.GetType() == eTT_SemiColon)
    {
      idx = i + 1;
      return;
    }
    else if (token.GetType() == eTT_CurlyBraceOpen)
    {
      idx = i + 1;
      SkipBlock(tokenizer, idx);
      return;
    }
  }
  throw ParseException(__FILE__, __LINE__, "Unable to skip enum. Neither ';' nor '{' was found.", line, column);
}

void Parser::SkipBlock(Tokenizer &tokenizer, size_t &idx)
{
  auto        tokens = tokenizer.GetTokens();
  for (size_t in     = tokens.size(); idx < in;)
  {
    CHECK_IDX(tokens, idx);
    Token &token = tokens[idx++];
    if (token.GetType() == eTT_CurlyBraceOpen)
    {
      SkipBlock(tokenizer, idx);
    }
    else if (token.GetType() == eTT_CurlyBraceClose)
    {
      return;
    }
  }
}

std::string Parser::GetName(Tokenizer &tokenizer, size_t &idx)
{
  std::string name;
  auto        tokens = tokenizer.GetTokens();
  CHECK_IDX(tokens, idx);
  Token &token = tokens[idx++];

  if (token.GetType() == eTT_Identifier)
  {
    name += token.Get();
    while (true)
    {
      CHECK_IDX(tokens, idx);
      token = tokens[idx];
      if (token.GetType() == eTT_DoubleColon)
      {
        idx++;
        name += token.Get();
      }
      else
      {
        break;
      }
      CHECK_IDX(tokens, idx);
      token = tokens[idx];
      if (token.GetType() == eTT_Identifier)
      {
        idx++;
        name += token.Get();
      }
      else
      {
        break;
      }
    }
  }
  return name;
}


std::string Parser::ReverseName(Tokenizer &tokenizer, size_t &idx)
{
  std::string name;
  auto        tokens        = tokenizer.GetTokens();
  int         angleBrackets = 0;
  while (true)
  {
    CHECK_IDX(tokens, idx - 1);
    Token &token = tokens[--idx];
    if (token.GetType() == eTT_AngleBracketClose)
    {
      name = ">" + name;
      angleBrackets++;
    }
    else if (token.GetType() == eTT_AngleBracketOpen)
    {
      name = "<" + name;
      angleBrackets--;
    }

    if (angleBrackets == 0)
    {

      if (token.GetType() == eTT_Identifier)
      {
        name = token.Get() + name;
      }
      CHECK_IDX(tokens, idx - 1);
      token = tokens[--idx];
      if (token.GetType() == eTT_DoubleColon)
      {
        name = "::" + name;
        continue;
      }

      break;
    }
    else
    {
      name = token.Get() + name;
      if (token.Get() == std::string("operator"))
      {
        break;
      }
    }
  }
  return name;
}


TypeDef Parser::GetType(Tokenizer &tokenizer, size_t &idx)
{
  auto    tokens         = tokenizer.GetTokens();
  TypeDef def;
  bool    haveIdentifier = false;
  while (true)
  {
    CHECK_IDX(tokens, idx);
    Token &token = tokens[idx++];
    if (token.GetType() == eTT_Virtual)
    {
      continue;
    }
    if (token.GetType() == eTT_Identifier)
    {
      if (token.Get() == std::string("CS_FORCEINLINE")
          || token.Get() == std::string("CS_NODISCARD")
          || token.Get() == std::string("inline")
          || token.Get() == std::string("__forceinline"))
      {
        continue;
      }
    }
    if (token.GetType() == eTT_Template)
    {
      int numAngular = 0;
      while (idx < tokens.size())
      {
        CHECK_IDX(tokens, idx);
        token = tokens[idx++];
        if (token.GetType() == eTT_AngleBracketOpen)
        {
          numAngular++;
        }
        else if (token.GetType() == eTT_AngleBracketClose)
        {
          numAngular--;
          if (numAngular == 0)
          {
            break;
          }
        }
      }
      continue;
    }
    else if (token.GetType() == eTT_Friend)
    {
      continue;
    }
    else if (token.GetType() == eTT_Class || token.GetType() == eTT_Struct)
    {
      continue;
    }
    if (token.GetType() == eTT_AngleBracketOpen)
    {
      while (idx < tokens.size())
      {
        TypeDef subType = GetType(tokenizer, idx);
        def.Add(subType);

        CHECK_IDX(tokens, idx);
        token = tokens[idx];
        if (token.GetType() == eTT_Comma)
        {
          idx++;
          continue;
        }
        else if (token.GetType() == eTT_AngleBracketClose)
        {
          idx++;
          break;
        }
        else if (token.GetType() == eTT_ParenOpen)
        {
          int numParens = 0;
          while (idx < tokens.size())
          {
            CHECK_IDX(tokens, idx);
            token = tokens[idx++];
            if (token.GetType() == eTT_ParenOpen)
            {
              numParens++;
            }
            else if (token.GetType() == eTT_ParenClose)
            {
              numParens--;
              if (numParens == 0)
              {
                break;
              }
            }
          }

        }
        else
        {
          idx++;
          //throw ParseException(__FILE__, __LINE__, "Unexpected token '" + token.Get() + "' -> " + std::to_string(token.GetType()));
        }
      }
      continue;
    }
    else if (token.GetType() == eTT_BracketOpen)
    {
      CHECK_IDX(tokens, idx+3);
      if (tokens[idx].GetType() == eTT_BracketOpen
          && tokens[idx + 1].GetType() == eTT_Identifier && tokens[idx + 1].Get() == "nodiscard"
          && tokens[idx + 2].GetType() == eTT_BracketClose
          && tokens[idx + 3].GetType() == eTT_BracketClose)
      {
        idx += 4;
        continue;
      }
      CHECK_IDX(tokens, idx+5);
      if (tokens[idx].GetType() == eTT_BracketOpen
          && tokens[idx + 1].GetType() == eTT_Identifier && tokens[idx + 1].Get() == "nodiscard"
          && tokens[idx + 2].GetType() == eTT_ParenOpen
          && tokens[idx + 3].GetType() == eTT_ParenClose
          && tokens[idx + 4].GetType() == eTT_BracketClose
          && tokens[idx + 5].GetType() == eTT_BracketClose)
      {
        idx += 4;
        continue;
      }

    }

    if (token.GetType() == eTT_Identifier)
    {
      if (haveIdentifier)
      {
        --idx;
        break;
      }
      haveIdentifier = true;
      def.Add(token);
      while (true)
      {
        CHECK_IDX(tokens, idx);
        token = tokens[idx];
        if (token.GetType() == eTT_DoubleColon)
        {
          idx++;
          def.Add(token);
        }
        else if (token.GetType() == eTT_DotDotDot)
        {
          idx++;
        }
        else
        {
          break;
        }
        CHECK_IDX(tokens, idx);
        token = tokens[idx];
        if (token.GetType() == eTT_Identifier)
        {
          idx++;
          def.Add(token);
        }
        else
        {
          break;
        }
      }
    }
    else if (token.GetType() == eTT_Const
             || token.GetType() == eTT_Ampersand
             || token.GetType() == eTT_Asterisk
             || token.GetType() == eTT_DoubleAsterisk)
    {
      def.Add(token);
    }
    else
    {
      idx--;
      break;
    }
  }

  return def;
}

TypeDef Parser::ReverseType(Tokenizer &tokenizer, size_t &idx)
{
  auto    tokens        = tokenizer.GetTokens();
  int     angleBrackets = 0;
  TypeDef def;
  while (true)
  {
    CHECK_IDX(tokens, idx);
    Token &token = tokens[idx--];
    if (token.GetType() == eTT_AngleBracketClose)
    {
      while (idx > 0)
      {
        TypeDef subType = ReverseType(tokenizer, idx);
        def.AddFront(subType);

        CHECK_IDX(tokens, idx);
        token = tokens[idx];
        if (token.GetType() == eTT_Comma)
        {
          idx--;
          continue;
        }
        else if (token.GetType() == eTT_AngleBracketOpen)
        {
          idx--;
          break;
        }
      }
      continue;
    }

    if (token.GetType() == eTT_Identifier)
    {
      def.AddFront(token);
      while (true)
      {
        CHECK_IDX(tokens, idx);
        token = tokens[idx];
        if (token.GetType() == eTT_DoubleColon)
        {
          idx--;
          def.AddFront(token);
        }
        else
        {
          break;
        }
        CHECK_IDX(tokens, idx);
        token = tokens[idx];
        if (token.GetType() == eTT_Identifier)
        {
          idx--;
          def.AddFront(token);
        }
        else
        {
          break;
        }
      }
    }
    else if (token.GetType() == eTT_Const
             || token.GetType() == eTT_Ampersand
             || token.GetType() == eTT_Asterisk
             || token.GetType() == eTT_DoubleAsterisk)
    {
      def.AddFront(token);
    }
    else
    {
      idx++;
      break;
    }
  }

  return def;

}

CSMetaNode *Parser::ParseCSMeta(Tokenizer &tokenizer, size_t &idx, ASTNode *parent)
{
  auto tokens = tokenizer.GetTokens();
  CHECK_IDX(tokens, idx);
  Token      &token    = tokens[idx];
  uint32_t   line      = token.getLine();
  uint32_t   column    = token.getColumn();
  CSMetaNode *metaNode = nullptr;

  if (idx + 2 >= tokens.size())
  {
    throw ParseException(__FILE__, __LINE__, "Malformed CS_FUNCTION", line, column);
  }

  CHECK_IDX(tokens, idx + 1);
  if (tokens[idx + 1].GetType() != eTT_ParenOpen)
  {
    throw ParseException(__FILE__, __LINE__, "Malformed CS_FUNCTION", line, column);
  }
  idx++;

  switch (token.GetType())
  {
    case eTT_CS_CLASS:
      metaNode = new CSMetaNode(CSMetaNode::eMT_Class);
      break;
    case eTT_CS_PROPERTY:
      metaNode = new CSMetaNode(CSMetaNode::eMT_Property);
      break;
    case eTT_CS_FUNCTION:
      metaNode = new CSMetaNode(CSMetaNode::eMT_Function);
      break;
    default:
      idx++;
      return nullptr;
  }

  std::string value = "";
  std::string key   = "";
  for (size_t in    = tokens.size(); idx < in; idx++)
  {

    CHECK_IDX(tokens, idx);
    token = tokens[idx];
    switch (token.GetType())
    {
      case eTT_ParenOpen:
        break;
      case eTT_ParenClose:
        if (!key.empty() || !value.empty())
        {
          metaNode->Add(CSMetaNode::Attribute(key, value));
        }
        key   = "";
        value = "";
        idx++;
        if (idx < in)
        {
          CHECK_IDX(tokens, idx);
          token = tokens[idx];
          if (token.GetType() == eTT_SemiColon)
          {
            idx++;
          }
        }
        return metaNode;
      case eTT_Identifier:
      case eTT_OtherCode:
        value = token.Get();
        break;
      case eTT_Equal:
      case eTT_Colon:
        key   = value;
        value = "";
        break;
      case eTT_Comma:
        if (!key.empty() || !value.empty())
        {
          metaNode->Add(CSMetaNode::Attribute(key, value));
        }
        key   = "";
        value = "";
        break;
    }
  }
  throw ParseException(__FILE__, __LINE__, "Malformed CS_FUNCTION", line, column);

}

}
