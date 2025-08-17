#include "ParserTotal.hpp"

// Declarations by Gemini.
auto parsePatternLiteralNode(TokenStream &stream)
    -> std::unique_ptr<PatternLiteralNode>;
auto parsePatternIDNode(TokenStream &stream) -> std::unique_ptr<PatternIDNode>;
auto parsePatternPathBeginNode(TokenStream &stream)
    -> std::unique_ptr<PatternNode>; // 注意返回值是基类指针
auto parsePatternTupleNode(TokenStream &stream)
    -> std::unique_ptr<PatternTupleNode>;
auto parsePatternWildcardNode(TokenStream &stream)
    -> std::unique_ptr<PatternWildNode>;

auto parsePatternStructField(TokenStream &stream) -> PatternStructField;

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
  case TokenType::LEFT_PAREN:
    return parsePatternTupleNode(stream);
  case TokenType::UNDERSCORE:
    return parsePatternWildcardNode(stream);
  case TokenType::IDENTIFIER:
    if (stream.peekNum(1).type == TokenType::COLON_COLON) {
      return parsePatternPathBeginNode(stream);
    } else {
      /*Allow the ambiguity.*/
      return parsePatternIDNode(stream);
    }
  }
  throw std::runtime_error("No ID pattern.");
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

auto parsePatternPathBeginNode(TokenStream &stream)
    -> std::unique_ptr<PatternNode> {
  auto path = parsePathNode(stream);
  if (stream.peek().type == TokenType::LEFT_BRACKET) {
    stream.next();
    std::vector<PatternStructField> fields;
    if (stream.next().type == TokenType::RIGHT_BRACKET) {
      stream.next();
    } else {
      fields.push_back(parsePatternStructField(stream));
      while (stream.peek().type == TokenType::COMMA) {
        stream.next();
        if (stream.peek().type == TokenType::RIGHT_BRACKET) {
          break;
        }
        fields.push_back(parsePatternStructField(stream));
      }
      if (stream.peek().type != TokenType::RIGHT_BRACKET) {
        throw std::runtime_error("Expect right bracket after slice end.");
      }
      stream.next();
    }
    return std::make_unique<PatternStructNode>(std::move(fields));
  }
  return std::make_unique<PatternPathNode>(parsePathNode(stream));
}

auto parsePatternStructField(TokenStream &stream) -> PatternStructField {
  std::string identifer = stream.peek().content;
  std::unique_ptr<PatternNode> pattern;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    pattern = parsePatternNode(stream);
  }
  return {identifer, std::move(pattern)};
}

auto parsePatternTupleNode(TokenStream &stream)
    -> std::unique_ptr<PatternTupleNode> {
  stream.next();
  std::vector<std::unique_ptr<PatternNode>> patterns;
  if (stream.peek().type == TokenType::RIGHT_PAREN) {
    return std::make_unique<PatternTupleNode>(std::move(patterns));
  }
  patterns.push_back(parsePatternNode(stream));
  while (stream.peek().type == TokenType::COMMA) {
    stream.next();
    if (stream.peek().type == TokenType::RIGHT_BRACKET) {
      break;
    }
    patterns.push_back(parsePatternNode(stream));
  }
  if (stream.peek().type != TokenType::RIGHT_BRACKET) {
    throw std::runtime_error("Expect right bracket after slice end.");
  }
  stream.next();
  return std::make_unique<PatternTupleNode>(std::move(patterns));
}

auto parsePatternWildcardNode(TokenStream &stream)
    -> std::unique_ptr<PatternWildNode> {
  return std::make_unique<PatternWildNode>();
}