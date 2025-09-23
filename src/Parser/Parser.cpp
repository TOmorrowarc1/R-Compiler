#include "Parser.hpp"
#include "ParserTotal.hpp"
#include "exception.hpp"
#include "logger.hpp"

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTRootNode> {
  TokenStream stream(tokens);
  auto rootNode = parseRootNode(stream);
  if (stream.peek().type != TokenType::END_OF_FILE) {
    throw CompilerException("Expected EOF after parsing AST.",
                            stream.peek().line);
  }
  // LoggerPlant::getInstance().output();
  return rootNode;
}

auto parseRootNode(TokenStream &stream) -> std::unique_ptr<ASTRootNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseRootNode");
  std::vector<std::unique_ptr<ItemNode>> items;
  while (true) {
    Token token = stream.peek();
    if (token.type == TokenType::END_OF_FILE) {
      break;
    }
    items.push_back(parseItemNode(stream));
  }
  return std::make_unique<ASTRootNode>(std::move(items), Position(0));
}