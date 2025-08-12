#include "ParserTotal.hpp"

auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode> {
  return std::make_unique<TypePathNode>(std::move(parsePathNode(stream)));
}

auto parseTypeInferNode(TokenStream &stream) -> std::unique_ptr<TypeInferNode> {
  stream.next();
  return std::make_unique<TypeInferNode>();
}

auto parseTypeNeverNode(TokenStream &stream) -> std::unique_ptr<TypeNeverNode> {
  stream.next();
  return std::make_unique<TypeNeverNode>();
}

auto parseTypeLeftParentNode(TokenStream &stream) -> std::unique_ptr<TypeNode> {
  stream.next();
  std::vector<std::unique_ptr<TypeNode>> fields;
  if (stream.peek().type == TokenType::RIGHT_PAREN) {
    stream.next();
    return std::make_unique<TypeTupleNode>(std::move(fields));
  }
  auto type_node = std::move(parseTypeNode(stream));
  if (stream.peek().type == TokenType::COMMA) {
    stream.next();
    while (stream.peek().type != TokenType::RIGHT_PAREN) {
      fields.push_back(std::move(parseTypeNode(stream)));
      if (stream.next().type != TokenType::COMMA) {
        throw std::runtime_error("Expect a comma after a type in tuple.");
      }
    }
    stream.next();
    return std::make_unique<TypeTupleNode>(std::move(fields));
  }
  return std::make_unique<TypeParentNode>(std::move(type_node));
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
  case TokenType::COLON_COLON:
    return parseTypePathNode(stream);
  case TokenType::LEFT_PAREN:
    return parseTypeLeftParentNode(stream);
  case TokenType::UNDERSCORE:
    return parseTypeInferNode(stream);
  case TokenType::NOT:
    return parseTypeNeverNode(stream);
  case TokenType::LEFT_BRACKET:
    return parseTypeLeftBracketNode(stream);
  }
  throw std::runtime_error("Unexpected token type for type node: " +
                           stream.peek().content);
  return nullptr;
}