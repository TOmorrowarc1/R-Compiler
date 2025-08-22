#include "ParserTotal.hpp"
#include "exception.hpp"

// By Geimini.
auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode>;
auto parseTypeArrayNode(TokenStream &stream) -> std::unique_ptr<TypeArrayNode>;
auto parseTypeReferNode(TokenStream &stream) -> std::unique_ptr<TypeReferNode>;

auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode> {
  return std::make_unique<TypePathNode>(parsePathNode(stream));
}

auto parseTypeArrayNode(TokenStream &stream) -> std::unique_ptr<TypeArrayNode> {
  stream.next();
  bool is_array = false;
  std::unique_ptr<ExprNode> expr_node;
  std::unique_ptr<TypeNode> type_node = parseTypeNode(stream);
  if (stream.peek().type != TokenType::SEMICOLON) {
    throw CompilerException("Expect ; after type in array type.",
                            stream.peek().line);
  }
  stream.next();
  expr_node = parseExprNode(stream);
  if (stream.peek().type != TokenType::RIGHT_BRACKET) {
    throw CompilerException("Expect ] at the end of array type or slice.",
                            stream.peek().line);
  }
  stream.next();
  return std::make_unique<TypeArrayNode>(std::move(type_node),
                                         std::move(expr_node));
}

auto parseTypeReferNode(TokenStream &stream) -> std::unique_ptr<TypeReferNode> {
  stream.next();
  bool is_mutable = false;
  if (stream.peek().type == TokenType::MUT) {
    is_mutable = true;
    stream.next();
  }
  return std::make_unique<TypeReferNode>(parseTypeNode(stream), is_mutable);
}

auto parseTypeNode(TokenStream &stream) -> std::unique_ptr<TypeNode> {
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
  case TokenType::SELF_TYPE:
    return parseTypePathNode(stream);
  case TokenType::LEFT_BRACKET:
    return parseTypeArrayNode(stream);
  case TokenType::AND:
    return parseTypeReferNode(stream);
  }
  throw CompilerException("Unexpected token type for type node",
                          stream.peek().line);
  return nullptr;
}