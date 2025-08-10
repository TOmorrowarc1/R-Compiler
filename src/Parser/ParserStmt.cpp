#include "ParserTotal.hpp"

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