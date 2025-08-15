#pragma once
#include "StmtANode.hpp"

class PatternOneNode;
class TypeNode;
class ExprNode;
class ExprBlockNode;

class StmtLetNode : public StmtNode {
private:
  std::unique_ptr<PatternOneNode> pattern_;
  std::unique_ptr<TypeNode> type_;
  std::unique_ptr<ExprNode> init_value_;
  std::unique_ptr<ExprBlockNode> else_body_;

public:
  StmtLetNode(std::unique_ptr<PatternOneNode> &&pattern,
              std::unique_ptr<TypeNode> &&type,
              std::unique_ptr<ExprNode> &&init_value,
              std::unique_ptr<ExprBlockNode> &&else_body);
  ~StmtLetNode();
  void accept(Visitor &visitor) override;
};