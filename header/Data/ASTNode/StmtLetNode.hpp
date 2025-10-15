#pragma once
#include "StmtANode.hpp"

class PatternNode;
class TypeNode;
class ExprNode;

class StmtLetNode : public StmtNode {
public:
  std::unique_ptr<PatternNode> pattern_;
  std::unique_ptr<TypeNode> type_;
  std::unique_ptr<ExprNode> init_value_;

  StmtLetNode(std::unique_ptr<PatternNode> &&pattern,
              std::unique_ptr<TypeNode> &&type,
              std::unique_ptr<ExprNode> &&init_value, Position position);
  ~StmtLetNode();
  void accept(Visitor &visitor) override;
};