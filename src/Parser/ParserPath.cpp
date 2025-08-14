#include "ParserTotal.hpp"

auto parsePathNode(TokenStream &stream) -> std::unique_ptr<PathNode> {
  if (stream.peek().type == TokenType::LESS_THAN) {
    return parsePathQualifiedNode(stream);
  } else if (stream.peek().type == TokenType::IDENTIFIER ||
             stream.peek().type == TokenType::COLON_COLON) {
    return parsePathSimpleNode(stream);
  }
  throw std::runtime_error("Unexpected token in path parsing");
  return nullptr;
}

auto parsePathSimpleNode(TokenStream &stream)
    -> std::unique_ptr<PathSimpleNode> {
  std::vector<PathSegment> segments;
  if (stream.peek().type == TokenType::COLON_COLON) {
    stream.next();
    segments.push_back({PathSegmentType::CRATE, ""});
  }
  segments.push_back(parsePathSegment(stream));
  while (stream.peek().type != TokenType::COLON_COLON) {
    stream.next();
    segments.push_back(parsePathSegment(stream));
  }
  return std::make_unique<PathSimpleNode>(std::move(segments));
}

auto parsePathQualifiedNode(TokenStream &stream)
    -> std::unique_ptr<PathQualifiedNode> {
  stream.next();
  std::vector<PathSegment> segments;
  std::unique_ptr<PathSimpleNode> parent;
  std::unique_ptr<TypeNode> type;
  type = parseTypeNode(stream);
  if (stream.peek().type == TokenType::AS) {
    stream.next();
    parent = parsePathSimpleNode(stream);
  }
  if (stream.peek().type != TokenType::GREATER_THAN) {
    throw std::runtime_error("Expected '>' in path qualified node parsing");
  }
  stream.next();
  if (stream.peek().type != TokenType::COLON_COLON) {
    throw std::runtime_error("Expected '::' in path qualified node parsing");
  }
  while (stream.peek().type == TokenType::COLON_COLON) {
    stream.next();
    segments.push_back(parsePathSegment(stream));
  }
  return std::make_unique<PathQualifiedNode>(
      std::move(segments), std::move(parent), std::move(type));
}

auto parsePathSegment(TokenStream &stream) -> PathSegment {
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
    return {PathSegmentType::IDENTIFER, stream.next().content};
  case TokenType::SELF:
    stream.next();
    return {PathSegmentType::SELF, ""};
  case TokenType::SELF_TYPE:
    stream.next();
    return {PathSegmentType::SELF_TYPE, ""};
  case TokenType::SUPER:
    stream.next();
    return {PathSegmentType::SUPER, ""};
  case TokenType::CRATE:
    stream.next();
    return {PathSegmentType::CRATE, ""};
  }
  throw std::runtime_error("Unexpected token in path simple node parsing");
  return {PathSegmentType::IDENTIFER, ""};
}