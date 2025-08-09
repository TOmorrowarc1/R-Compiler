#include "Parser.hpp"
#include "ASTRootNode.hpp"
#include "ExprBlockNode.hpp"
#include "ExprCallNode.hpp"
#include "ExprFlowNode.hpp"
#include "ExprGroupNode.hpp"
#include "ExprIfNode.hpp"
#include "ExprLiteralNode.hpp"
#include "ExprLoopNode.hpp"
#include "ExprOperBinaryNode.hpp"
#include "ExprOperUnaryNode.hpp"
#include "ItemConstantNode.hpp"
#include "ItemFnNode.hpp"
#include "StmtEmptyNode.hpp"
#include "StmtExprNode.hpp"
#include "StmtItemNode.hpp"
#include "StmtLetNode.hpp"
#include "TypeInferNode.hpp"
#include "TypeNeverNode.hpp"
#include "TypeParentNode.hpp"
#include "TypePathNode.hpp"
#include "token.hpp"
#include <stdexcept>

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTRootNode>;
auto parseRootNode(TokenStream &stream) -> std::unique_ptr<ASTRootNode>;

auto parseItemNode(TokenStream &stream) -> std::unique_ptr<ItemNode>;
auto parseItemFnNode(TokenStream &stream) -> std::unique_ptr<ItemNode>;
auto parseFnParameters(TokenStream &stream) -> std::vector<FnParameter>;
auto parseFnParameter(TokenStream &stream) -> FnParameter;
auto parseItemConstNode(TokenStream &stream) -> std::unique_ptr<ItemNode>;

auto parseStmtNode(TokenStream &stream) -> std::unique_ptr<StmtNode>;
auto parseStmtItemNode(TokenStream &stream) -> std::unique_ptr<StmtItemNode>;
auto parseStmtLetNode(TokenStream &stream) -> std::unique_ptr<StmtLetNode>;
auto parseStmtExprNode(TokenStream &stream) -> std::unique_ptr<StmtExprNode>;
auto parseStmtEmptyNode(TokenStream &stream) -> std::unique_ptr<StmtEmptyNode>;

auto parseExprNode(TokenStream &stream) -> std::unique_ptr<ExprNode>;
auto parseExprBlockInNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockInNode>;
auto parseExprBlockOutNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockOutNode>;
auto parseExprBlockNode(TokenStream &stream) -> std::unique_ptr<ExprBlockNode>;
auto parseExprBlockConstNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockConstNode>;
auto parseExprLoopNode(TokenStream &stream) -> std::unique_ptr<ExprLoopNode>;
auto parseExprWhileNode(TokenStream &stream) -> std::unique_ptr<ExprWhileNode>;
auto parseExprIfNode(TokenStream &stream) -> std::unique_ptr<ExprIfNode>;
auto parseExprLiteralNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralNode>;
auto parseExprLiteralIntNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralIntNode>;
auto parseExprLiteralBoolNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralBoolNode>;
auto parseExprLiteralCharNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralCharNode>;
auto parseExprLiteralStringNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralStringNode>;
auto parseExprLiteralByteNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralByteNode>;
auto parseExprOperBinaryNode(TokenStream &stream)
    -> std::unique_ptr<ExprOperBinaryNode>;
auto parseExprOperUnaryNode(TokenStream &stream)
    -> std::unique_ptr<ExprOperUnaryNode>;
auto parseExprGroupNode(TokenStream &stream) -> std::unique_ptr<ExprGroupNode>;
auto parseExprCallNode(TokenStream &stream) -> std::unique_ptr<ExprCallNode>;
auto parseExprBreakNode(TokenStream &stream) -> std::unique_ptr<ExprBreakNode>;
auto parseExprReturnNode(TokenStream &stream)
    -> std::unique_ptr<ExprReturnNode>;
auto parseExprContinueNode(TokenStream &stream)
    -> std::unique_ptr<ExprContinueNode>;

auto parseTypeNode(TokenStream &stream) -> std::unique_ptr<TypeNode>;
auto parseTypeInferNode(TokenStream &stream) -> std::unique_ptr<TypeInferNode>;
auto parseTypeNeverNode(TokenStream &stream) -> std::unique_ptr<TypeNeverNode>;
auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode>;
auto parseTypeParentNode(TokenStream &stream)
    -> std::unique_ptr<TypeParentNode>;

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTRootNode> {
  TokenStream stream(tokens);
  auto rootNode = parseRootNode(stream);
  if (stream.peek().type != TokenType::END_OF_FILE) {
    throw std::runtime_error("Unexpected tokens after parsing AST.");
  }
  return rootNode;
}

auto parseRootNode(TokenStream &stream) -> std::unique_ptr<ASTRootNode> {
  std::vector<std::unique_ptr<ItemNode>> items;
  while (true) {
    Token token = stream.peek();
    if (token.type == TokenType::END_OF_FILE) {
      break;
    }
    auto itemNode = parseItemNode(stream);
    items.push_back(std::move(itemNode));
  }
  return std::make_unique<ASTRootNode>(std::move(items));
}

auto parseItemNode(TokenStream &stream) -> std::unique_ptr<ItemNode> {
  switch (stream.peek().type) {
  case TokenType::FN:
    return parseItemFnNode(stream);
  case TokenType::CONST:
    return parseItemConstNode(stream);
  default:
    throw std::runtime_error("Unexpected token type for ItemNode");
  }
  return nullptr;
}

auto parseItemFnNode(TokenStream &stream) -> std::unique_ptr<ItemNode> {
  stream.next();
  std::vector<FnParameter> parameters;
  std::string function_name;
  std::unique_ptr<TypeNode> returnType;
  std::unique_ptr<ExprBlockNode> body;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Expected identifier after 'fn'");
  }
  function_name = stream.next().content;
  if (stream.peek().type != TokenType::LEFT_PAREN) {
    throw std::runtime_error("Expected '(' after function name");
  }
  stream.next();
  parameters = parseFnParameters(stream);
  if (stream.peek().type != TokenType::RIGHT_PAREN) {
    throw std::runtime_error("Expected ')' after function parameters");
  }
  stream.next();
  if (stream.peek().type == TokenType::ARROW) {
    stream.next();
    returnType = std::move(parseTypeNode(stream));
  } else {
    returnType = nullptr;
  }
  if (stream.peek().type != TokenType::LEFT_BRACE) {
    throw std::runtime_error("Expected '{' after function signature");
  }
  body = std::move(parseExprBlockNode(stream));
  return std::make_unique<ItemFnNode>(function_name, std::move(returnType),
                                      std::move(parameters), std::move(body));
}

auto parseFnParameters(TokenStream &stream) -> std::vector<FnParameter> {
  std::vector<FnParameter> parameters;
  if (!(stream.peek().type == TokenType::RIGHT_PAREN)) {
    while (true) {
      FnParameter param = parseFnParameter(stream);
      parameters.push_back(std::move(param));
      if (stream.peek().type == TokenType::COMMA) {
        stream.next();
      } else if (stream.peek().type == TokenType::RIGHT_PAREN) {
        break;
      } else {
        throw std::runtime_error("Unexpected token in function parameters");
      }
    }
  }
  return parameters;
}

auto parseFnParameter(TokenStream &stream) -> FnParameter {
  FnParameter param;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Expected identifier for function parameter");
  }
  param.name = stream.next().content;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    param.type = std::move(parseTypeNode(stream));
  } else {
    throw std::runtime_error("Expected ':' after parameter name");
  }
  return param;
}

auto parseItemConstNode(TokenStream &stream) -> std::unique_ptr<ItemNode> {
  stream.next();
  std::string name;
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ExprBlockNode> value;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Expected identifier after 'const'");
  }
  name = stream.next().content;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    type = std::move(parseTypeNode(stream));
  } else {
    throw std::runtime_error("Expected ':' after constant name");
  }
  if (stream.peek().type != TokenType::EQUAL) {
    throw std::runtime_error("Expected '=' after constant type");
  }
  stream.next();
  value = std::move(parseExprBlockNode(stream));
  return std::make_unique<ItemConstantNode>(name, std::move(type),
                                            std::move(value));
}

auto parseStmtNode(TokenStream &stream) -> std::unique_ptr<StmtNode> {
  switch (stream.peek().type) {
  case TokenType::SEMICOLON:
    return parseStmtEmptyNode(stream);
  case TokenType::LET:
    return parseStmtLetNode(stream);
  case TokenType::FN:
  case TokenType::CONST:
    return parseStmtItemNode(stream);
  default:
    return parseStmtExprNode(stream);
  }
}

auto parseStmtItemNode(TokenStream &stream) -> std::unique_ptr<StmtItemNode> {
  auto itemNode = parseItemNode(stream);
  return std::make_unique<StmtItemNode>(std::move(itemNode));
}

auto parseStmtLetNode(TokenStream &stream) -> std::unique_ptr<StmtLetNode> {
  stream.next();
  std::string identifier;
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ExprNode> init_value;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Expected identifier after 'let'");
  }
  identifier = stream.next().content;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    type = std::move(parseTypeNode(stream));
  }
  if (stream.peek().type == TokenType::EQUAL) {
    stream.next();
    init_value = std::move(parseExprNode(stream));
  }
  return std::make_unique<StmtLetNode>(identifier, std::move(type),
                                       std::move(init_value));
}

auto parseStmtExprNode(TokenStream &stream) -> std::unique_ptr<StmtExprNode> {
  std::unique_ptr<ExprNode> exprNode;
  auto token = stream.peek();
  if (token.type == TokenType::LEFT_BRACE || token.type == TokenType::CONST ||
      token.type == TokenType::WHILE || token.type == TokenType::LOOP ||
      token.type == TokenType::IF) {
    exprNode = std::move(parseExprBlockInNode(stream));
    if (stream.peek().type == TokenType::SEMICOLON) {
      stream.next();
    }
  } else {
    exprNode = std::move(parseExprBlockOutNode(stream));
    if (stream.peek().type != TokenType::SEMICOLON) {
      throw std::runtime_error("Expected ';' after expression");
    }
    stream.next();
  }
  return std::make_unique<StmtExprNode>(std::move(exprNode));
}

auto parseStmtEmptyNode(TokenStream &stream) -> std::unique_ptr<StmtEmptyNode> {
  stream.next();
  return std::make_unique<StmtEmptyNode>();
}