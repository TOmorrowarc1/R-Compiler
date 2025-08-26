#include "ParserTotal.hpp"
#include "exception.hpp"

// Declarations by Gemini.
auto parsePatternLiteralNode(TokenStream &stream)
    -> std::unique_ptr<PatternLiteralNode>;
auto parsePatternIDNode(TokenStream &stream) -> std::unique_ptr<PatternIDNode>;
auto parsePatternPathNode(TokenStream &stream)
    -> std::unique_ptr<PatternPathNode>;
auto parsePatternReferNode(TokenStream &stream)
    -> std::unique_ptr<PatternReferNode>;
auto parsePatternWildcardNode(TokenStream &stream)
    -> std::unique_ptr<PatternWildNode>;

auto parsePatternNode(TokenStream &stream) -> std::unique_ptr<PatternNode> {
  Position position = stream.peek().line;
  switch (stream.peek().type) {
  case TokenType::MINUS:
  case TokenType::CHARLITERAL:
  case TokenType::STRINGLITERAL:
  case TokenType::RAWSTRINGLITERAL:
  case TokenType::CSTRINGLITERAL:
  case TokenType::RAWCSTRINGLITERAL:
  case TokenType::INTEGERLITERAL:
  case TokenType::TRUE:
  case TokenType::FALSE:
    return parsePatternLiteralNode(stream);
  case TokenType::UNDERSCORE:
    return parsePatternWildcardNode(stream);
  case TokenType::AND:
    return parsePatternReferNode(stream);
  case TokenType::IDENTIFIER:
    if (stream.peekNum(1).type == TokenType::COLON_COLON) {
      return parsePatternPathNode(stream);
    } else {
      /*Allow the ambiguity.*/
      return parsePatternIDNode(stream);
    }
  case TokenType::MUT:
    return parsePatternIDNode(stream);
  }
  throw CompilerException("No ID pattern.", position);
  return nullptr;
}

auto parsePatternLiteralNode(TokenStream &stream)
    -> std::unique_ptr<PatternLiteralNode> {
  Position position = stream.peek().line;
  bool minus = false;
  if (stream.peek().type == TokenType::MINUS) {
    minus = true;
    stream.next();
  }
  auto expr = parseExprLiteralNode(stream);
  return std::make_unique<PatternLiteralNode>(std::move(expr), minus, position);
}

auto parsePatternIDNode(TokenStream &stream) -> std::unique_ptr<PatternIDNode> {
  Position position = stream.peek().line;
  std::unique_ptr<PatternNode> pattern;
  std::string name;
  PatternIDNode::IDType id_type = PatternIDNode::IDType::PLAIN;
  if (stream.peek().type == TokenType::MUT) {
    id_type = PatternIDNode::IDType::MUT;
    stream.next();
  }
  name = stream.next().content;
  if (stream.peek().type == TokenType::AT) {
    pattern = parsePatternNode(stream);
  }
  return std::make_unique<PatternIDNode>(name, std::move(pattern), position);
}

auto parsePatternPathNode(TokenStream &stream)
    -> std::unique_ptr<PatternPathNode> {
  Position position = stream.peek().line;
  auto path = parsePathNode(stream);
  return std::make_unique<PatternPathNode>(std::move(path), position);
}

auto parsePatternReferNode(TokenStream &stream)
    -> std::unique_ptr<PatternReferNode> {
  Position position = stream.peek().line;
  stream.next();
  bool is_mutable = false;
  if (stream.peek().type == TokenType::MUT) {
    is_mutable = true;
    stream.next();
  }
  auto pattern = parsePatternNode(stream);
  return std::make_unique<PatternReferNode>(std::move(pattern), is_mutable,
                                            position);
}

auto parsePatternWildcardNode(TokenStream &stream)
    -> std::unique_ptr<PatternWildNode> {
  Position position = stream.peek().line;
  stream.next();
  return std::make_unique<PatternWildNode>(position);
}