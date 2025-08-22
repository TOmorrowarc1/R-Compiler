#include "ParserTotal.hpp"
#include "exception.hpp"

auto parsePathSegment(TokenStream &stream) -> PathSegment;

auto parsePathNode(TokenStream &stream) -> std::unique_ptr<PathNode> {
  Position position = stream.peek().line;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw CompilerException("Unexpected token in path parsing", position);
  }
  std::vector<PathSegment> segments;
  segments.push_back(parsePathSegment(stream));
  if (stream.peek().type == TokenType::COLON_COLON) {
    stream.next();
    segments.push_back(parsePathSegment(stream));
  }
  return std::make_unique<PathNode>(std::move(segments));
}

auto parsePathSegment(TokenStream &stream) -> PathSegment {
  Position position = stream.peek().line;
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
    return {PathSegmentType::IDENTIFER, stream.next().content};
  case TokenType::SELF:
    stream.next();
    return {PathSegmentType::SELF, ""};
  case TokenType::SELF_TYPE:
    stream.next();
    return {PathSegmentType::SELF_TYPE, ""};
  }
  throw CompilerException("Unexpected token in path simple node parsing",
                          position);
  return {PathSegmentType::IDENTIFER, ""};
}