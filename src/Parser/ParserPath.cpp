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
    segments.push_back({PathSegmentType::CRATE, ""});
    stream.next();
  }
  while (true) {
    switch (stream.peek().type) {
    case TokenType::IDENTIFIER:
      segments.push_back({PathSegmentType::IDENTIFER, stream.next().content});
      break;
    case TokenType::SELF:
      segments.push_back({PathSegmentType::SELF, ""});
      stream.next();
      break;
    case TokenType::SELF_TYPE:
      segments.push_back({PathSegmentType::SELF_TYPE, ""});
      stream.next();
      break;
    case TokenType::SUPER:
      segments.push_back({PathSegmentType::SUPER, ""});
      stream.next();
      break;
    case TokenType::CRATE:
      segments.push_back({PathSegmentType::CRATE, ""});
      stream.next();
      break;
    default:
      throw std::runtime_error("Unexpected token in path simple node parsing");
    }
    if (stream.peek().type != TokenType::COLON_COLON) {
      break;
    }
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
  stream.next();
  while (true) {
    switch (stream.peek().type) {
    case TokenType::IDENTIFIER:
      segments.push_back({PathSegmentType::IDENTIFER, stream.next().content});
      break;
    case TokenType::SELF:
      segments.push_back({PathSegmentType::SELF, ""});
      stream.next();
      break;
    case TokenType::SELF_TYPE:
      segments.push_back({PathSegmentType::SELF_TYPE, ""});
      stream.next();
      break;
    case TokenType::SUPER:
      segments.push_back({PathSegmentType::SUPER, ""});
      stream.next();
      break;
    case TokenType::CRATE:
      segments.push_back({PathSegmentType::CRATE, ""});
      stream.next();
      break;
    default:
      throw std::runtime_error("Unexpected token in path simple node parsing");
    }
    if (stream.peek().type != TokenType::COLON_COLON) {
      break;
    }
  }
  return std::make_unique<PathQualifiedNode>(
      std::move(segments), std::move(parent), std::move(type));
}