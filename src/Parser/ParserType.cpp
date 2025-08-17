#include "ParserTotal.hpp"

// By Geimini.
auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode>;
auto parseTypeInferNode(TokenStream &stream) -> std::unique_ptr<TypeInferNode>;
auto parseTypeLeftBracketNode(TokenStream &stream) -> std::unique_ptr<TypeNode>;

auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode> {
  return std::make_unique<TypePathNode>(std::move(parsePathNode(stream)));
}

auto parseTypeInferNode(TokenStream &stream) -> std::unique_ptr<TypeInferNode> {
  stream.next();
  return std::make_unique<TypeInferNode>();
}

auto parseTypeLeftBracketNode(TokenStream &stream)
    -> std::unique_ptr<TypeNode> {
  stream.next();
  bool is_array = false;
  std::unique_ptr<ExprNode> expr_node;
  std::unique_ptr<TypeNode> type_node = std::move(parseTypeNode(stream));
  if (stream.peek().type == TokenType::SEMICOLON) {
    stream.next();
    expr_node = std::move(parseExprNode(stream));
    is_array = true;
  }
  if (stream.next().type != TokenType::RIGHT_BRACKET) {
    throw std::runtime_error("Expect ] at the end of array type or slice.");
  }
  if (is_array) {
    return std::make_unique<TypeArrayNode>(std::move(type_node),
                                           std::move(expr_node));
  }
  return std::make_unique<TypeSliceNode>(std::move(type_node));
}

auto parseTypeNode(TokenStream &stream) -> std::unique_ptr<TypeNode> {
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
    return parseTypePathNode(stream);
  case TokenType::UNDERSCORE:
    return parseTypeInferNode(stream);
  case TokenType::LEFT_BRACKET:
    return parseTypeLeftBracketNode(stream);
  }
  throw std::runtime_error("Unexpected token type for type node: " +
                           stream.peek().content);
  return nullptr;
}