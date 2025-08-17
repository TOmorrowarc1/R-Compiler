#include "ParserTotal.hpp"

// By Gemini.
auto parseStmtEmptyNode(TokenStream &stream) -> std::unique_ptr<StmtEmptyNode>;
auto parseStmtLetNode(TokenStream &stream) -> std::unique_ptr<StmtLetNode>;
auto parseStmtItemNode(TokenStream &stream) -> std::unique_ptr<StmtItemNode>;
auto parseStmtExprNode(TokenStream &stream) -> std::unique_ptr<StmtExprNode>;

auto parseStmtNode(TokenStream &stream) -> std::unique_ptr<StmtNode> {
  switch (stream.peek().type) {
  case TokenType::SEMICOLON:
    return parseStmtEmptyNode(stream);
  case TokenType::LET:
    return parseStmtLetNode(stream);
  case TokenType::MOD:
  case TokenType::FN:
  case TokenType::CONST:
  case TokenType::STRUCT:
  case TokenType::ENUM:
  case TokenType::IMPL:
    return parseStmtItemNode(stream);
  default:
    return parseStmtExprNode(stream);
  }
}

auto parseStmtEmptyNode(TokenStream &stream) -> std::unique_ptr<StmtEmptyNode> {
  stream.next();
  return std::make_unique<StmtEmptyNode>();
}
auto parseStmtLetNode(TokenStream &stream) -> std::unique_ptr<StmtLetNode> {
  stream.next();
  std::unique_ptr<PatternNode> pattern;
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ExprNode> init_value;
  std::unique_ptr<ExprBlockNode> else_body;
  pattern = parsePatternNode(stream);
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    type = parseTypeNode(stream);
  }
  if (stream.peek().type == TokenType::EQUAL) {
    stream.next();
    init_value = parseExprNode(stream);
  }
  return std::make_unique<StmtLetNode>(std::move(pattern), std::move(type),
                                       std::move(init_value));
}

auto parseStmtItemNode(TokenStream &stream) -> std::unique_ptr<StmtItemNode> {
  auto itemNode = parseItemNode(stream);
  return std::make_unique<StmtItemNode>(std::move(itemNode));
}

auto parseStmtExprNode(TokenStream &stream) -> std::unique_ptr<StmtExprNode> {
  std::unique_ptr<ExprNode> exprNode;
  exprNode = parseExprNode(stream);
  if (stream.peek().type == TokenType::SEMICOLON) {
    stream.next();
  } else {
    if (is_instance_of<ExprBlockOutNode, ExprNode>(exprNode)) {
      throw std::runtime_error("Every stmtExpr filled with a exprwithoutblock "
                               "end with a semicolon.");
    }
  }
  return std::make_unique<StmtExprNode>(std::move(exprNode));
}