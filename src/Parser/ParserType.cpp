#include "ParserTotal.hpp"

// By Geimini.
auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode>;
auto parseTypeArrayNode(TokenStream &stream) -> std::unique_ptr<TypeArrayNode>;

auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode> {
  return std::make_unique<TypePathNode>(std::move(parsePathNode(stream)));
}

auto parseTypeArrayNode(TokenStream &stream) -> std::unique_ptr<TypeArrayNode> {
  stream.next();
  bool is_array = false;
  std::unique_ptr<ExprNode> expr_node;
  std::unique_ptr<TypeNode> type_node = parseTypeNode(stream);
  if (stream.peek().type != TokenType::SEMICOLON) {
    throw std::runtime_error("Expect ; after type in array type.");
  }
  stream.next();
  expr_node = parseExprNode(stream);
  if (stream.peek().type != TokenType::RIGHT_BRACKET) {
    throw std::runtime_error("Expect ] at the end of array type or slice.");
  }
  stream.next();
  return std::make_unique<TypeArrayNode>(std::move(type_node),
                                         std::move(expr_node));
}

auto parseTypeNode(TokenStream &stream) -> std::unique_ptr<TypeNode> {
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
    return parseTypePathNode(stream);
  case TokenType::LEFT_BRACKET:
    return parseTypeArrayNode(stream);
  }
  throw std::runtime_error("Unexpected token type for type node: " +
                           stream.peek().content);
  return nullptr;
}