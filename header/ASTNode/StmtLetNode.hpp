#pragma once
#include "StmtANode.hpp"

class PatternNode;
class TypeNode;
class ExprNode;

class StmtLetNode : public StmtNode {
private:
  std::unique_ptr<PatternNode> pattern_;
  std::unique_ptr<TypeNode> type_;
  std::unique_ptr<ExprNode> init_value_;

public:
  StmtLetNode(std::unique_ptr<PatternNode> &&pattern,
              std::unique_ptr<TypeNode> &&type,
              std::unique_ptr<ExprNode> &&init_value);
  ~StmtLetNode();
  void accept(Visitor &visitor) override;
};