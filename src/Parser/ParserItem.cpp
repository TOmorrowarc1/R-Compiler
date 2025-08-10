#include "ParserTotal.hpp"

auto parseItemNode(TokenStream &stream) -> std::unique_ptr<ItemNode> {
  switch (stream.peek().type) {
  case TokenType::FN:
    return parseItemFnNode(stream);
  case TokenType::CONST:
    return parseItemConstNode(stream);
  default:
    throw std::runtime_error("Unexpected token type for ItemNode");
  }
  return nullptr;
}

auto parseItemFnNode(TokenStream &stream) -> std::unique_ptr<ItemNode> {
  stream.next();
  std::vector<FnParameter> parameters;
  std::string function_name;
  std::unique_ptr<TypeNode> returnType;
  std::unique_ptr<ExprBlockNode> body;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Expected identifier after 'fn'");
  }
  function_name = stream.next().content;
  if (stream.peek().type != TokenType::LEFT_PAREN) {
    throw std::runtime_error("Expected '(' after function name");
  }
  stream.next();
  parameters = parseFnParameters(stream);
  if (stream.peek().type != TokenType::RIGHT_PAREN) {
    throw std::runtime_error("Expected ')' after function parameters");
  }
  stream.next();
  if (stream.peek().type == TokenType::ARROW) {
    stream.next();
    returnType = std::move(parseTypeNode(stream));
  } else {
    returnType = nullptr;
  }
  if (stream.peek().type != TokenType::LEFT_BRACE) {
    throw std::runtime_error("Expected '{' after function signature");
  }
  body = std::move(parseExprBlockNode(stream));
  return std::make_unique<ItemFnNode>(function_name, std::move(returnType),
                                      std::move(parameters), std::move(body));
}

auto parseFnParameters(TokenStream &stream) -> std::vector<FnParameter> {
  std::vector<FnParameter> parameters;
  if (!(stream.peek().type == TokenType::RIGHT_PAREN)) {
    while (true) {
      FnParameter param = parseFnParameter(stream);
      parameters.push_back(std::move(param));
      if (stream.peek().type == TokenType::COMMA) {
        stream.next();
      } else if (stream.peek().type == TokenType::RIGHT_PAREN) {
        break;
      } else {
        throw std::runtime_error("Unexpected token in function parameters");
      }
    }
  }
  return parameters;
}

auto parseFnParameter(TokenStream &stream) -> FnParameter {
  FnParameter param;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Expected identifier for function parameter");
  }
  param.name = stream.next().content;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    param.type = std::move(parseTypeNode(stream));
  } else {
    throw std::runtime_error("Expected ':' after parameter name");
  }
  return param;
}

auto parseItemConstNode(TokenStream &stream) -> std::unique_ptr<ItemNode> {
  stream.next();
  std::string name;
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ExprBlockNode> value;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Expected identifier after 'const'");
  }
  name = stream.next().content;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    type = std::move(parseTypeNode(stream));
  } else {
    throw std::runtime_error("Expected ':' after constant name");
  }
  if (stream.peek().type != TokenType::EQUAL) {
    throw std::runtime_error("Expected '=' after constant type");
  }
  stream.next();
  value = std::move(parseExprBlockNode(stream));
  return std::make_unique<ItemConstantNode>(name, std::move(type),
                                            std::move(value));
}