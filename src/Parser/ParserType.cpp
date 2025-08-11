#include "ParserTotal.hpp"

auto parseTypeInferNode(TokenStream &stream) -> std::unique_ptr<TypeInferNode> {
  stream.next();
  return std::make_unique<TypeInferNode>();
}

auto parseTypeNeverNode(TokenStream &stream) -> std::unique_ptr<TypeNeverNode> {
  return std::make_unique<TypeNeverNode>();
}

auto parseTypePathNode(TokenStream &stream) -> std::unique_ptr<TypePathNode> {
  std::vector<std::string> path;
  while (stream.peek().type == TokenType::IDENTIFIER) {
    path.push_back(stream.next().content);
    if (stream.peek().type != TokenType::COLON_COLON) {
      break;
    }
    stream.next();
  }
  return std::make_unique<TypePathNode>(std::move(path));
}

auto parseTypeParentNode(TokenStream &stream)
    -> std::unique_ptr<TypeParentNode> {
  stream.next();
  return std::make_unique<TypeParentNode>(std::move(parseTypeNode(stream)));
}

auto parseTypeNode(TokenStream &stream) -> std::unique_ptr<TypeNode> {
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
    return std::move(parseTypePathNode(stream));
  case TokenType::LEFT_PAREN:
    return std::move(parseTypeParentNode(stream));
  case TokenType::UNDERSCORE:
    return std::move(parseTypeInferNode(stream));
  case TokenType::NOT:
    return std::move(parseTypeNeverNode(stream));
  }
  throw std::runtime_error("Unexpected token type for type node: " +
                           stream.peek().content);
  return nullptr;
}