#include "ParserTotal.hpp"
#include "exception.hpp"

// By Geimini.
auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode>;
auto parseTypeArrayNode(TokenStream &stream) -> std::unique_ptr<TypeArrayNode>;
auto parseTypeReferNode(TokenStream &stream) -> std::unique_ptr<TypeReferNode>;
auto parseTypeUnitNode(TokenStream &stream) -> std::unique_ptr<TypeNode>;

auto parseTypeNode(TokenStream &stream) -> std::unique_ptr<TypeNode> {
  Position position = stream.peek().line;
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
  case TokenType::SELF_TYPE:
    return parseTypePathNode(stream);
  case TokenType::LEFT_BRACKET:
    return parseTypeArrayNode(stream);
  case TokenType::AND:
  case TokenType::LOGIC_AND:
    return parseTypeReferNode(stream);
  case TokenType::LEFT_PAREN:
    return parseTypeUnitNode(stream);
  }
  throw CompilerException("Unexpected token type for type node", position);
  return nullptr;
}

auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode> {
  Position position = stream.peek().line;
  return std::make_unique<TypePathNode>(parsePathNode(stream), position);
}

auto parseTypeArrayNode(TokenStream &stream) -> std::unique_ptr<TypeArrayNode> {
  Position position = stream.peek().line;
  stream.next();
  std::unique_ptr<ExprNode> expr_node;
  std::unique_ptr<TypeNode> type_node = parseTypeNode(stream);
  if (stream.peek().type != TokenType::SEMICOLON) {
    throw CompilerException("Expect ; after type in array type.", position);
  }
  stream.next();
  expr_node = parseExprNode(stream);
  if (stream.peek().type != TokenType::RIGHT_BRACKET) {
    throw CompilerException("Expect ] at the end of array type.", position);
  }
  stream.next();
  return std::make_unique<TypeArrayNode>(std::move(type_node),
                                         std::move(expr_node), position);
}

auto parseTypeReferNode(TokenStream &stream) -> std::unique_ptr<TypeReferNode> {
  Position position = stream.peek().line;
  auto token_type = stream.peek().type;
  stream.next();
  bool is_mutable = false;
  if (stream.peek().type == TokenType::MUT) {
    is_mutable = true;
    stream.next();
  }
  auto refer_node = std::make_unique<TypeReferNode>(parseTypeNode(stream),
                                                    is_mutable, position);
  if (token_type == TokenType::AND) {
    return refer_node;
  }
  return std::make_unique<TypeReferNode>(std::move(refer_node), false,
                                         position);
}

auto parseTypeUnitNode(TokenStream &stream) -> std::unique_ptr<TypeNode> {
  Position position = stream.peek().line;
  stream.next();
  if (stream.peek().type != TokenType::RIGHT_PAREN) {
    throw CompilerException("Expect ) in unit type.", position);
  }
  stream.next();
  return std::make_unique<TypeUnitNode>(position);
}