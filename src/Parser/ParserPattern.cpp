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
  }
  throw CompilerException("No ID pattern.", stream.peek().line);
  return nullptr;
}

auto parsePatternLiteralNode(TokenStream &stream)
    -> std::unique_ptr<PatternLiteralNode> {
  bool minus = false;
  if (stream.peek().type == TokenType::MINUS) {
    minus = true;
    stream.next();
  }
  auto expr = parseExprLiteralNode(stream);
  return std::make_unique<PatternLiteralNode>(std::move(expr), minus);
}

auto parsePatternIDNode(TokenStream &stream) -> std::unique_ptr<PatternIDNode> {
  std::unique_ptr<PatternNode> pattern;
  std::string name = stream.next().content;
  if (stream.peek().type == TokenType::AT) {
    pattern = parsePatternNode(stream);
  }
  return std::make_unique<PatternIDNode>(name, std::move(pattern));
}

auto parsePatternPathNode(TokenStream &stream)
    -> std::unique_ptr<PatternPathNode> {
  auto path = parsePathNode(stream);
  return std::make_unique<PatternPathNode>(std::move(path));
}

auto parsePatternReferNode(TokenStream &stream)
    -> std::unique_ptr<PatternReferNode> {
  stream.next();
  bool is_mutable = false;
  if (stream.peek().type == TokenType::MUT) {
    is_mutable = true;
    stream.next();
  }
  auto pattern = parsePatternNode(stream);
  return std::make_unique<PatternReferNode>(std::move(pattern), is_mutable);
}

auto parsePatternWildcardNode(TokenStream &stream)
    -> std::unique_ptr<PatternWildNode> {
  return std::make_unique<PatternWildNode>();
}