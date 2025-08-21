#include "Parser.hpp"
#include "ParserTotal.hpp"
#include "exception.hpp"

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTRootNode> {
  TokenStream stream(tokens);
  auto rootNode = parseRootNode(stream);
  if (stream.peek().type != TokenType::END_OF_FILE) {
    throw CompilerException("Expected EOF after parsing AST.",
                            stream.peek().line);
  }
  return rootNode;
}

auto parseRootNode(TokenStream &stream) -> std::unique_ptr<ASTRootNode> {
  std::vector<std::unique_ptr<ItemNode>> items;
  while (true) {
    Token token = stream.peek();
    if (token.type == TokenType::END_OF_FILE) {
      break;
    }
    auto itemNode = parseItemNode(stream);
    items.push_back(std::move(itemNode));
  }
  return std::make_unique<ASTRootNode>(std::move(items));
}