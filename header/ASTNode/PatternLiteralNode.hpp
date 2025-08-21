#pragma once
#include "PatternANode.hpp"

class ExprLiteralNode;

class PatternLiteralNode : public PatternNode {
public:
  std::unique_ptr<ExprLiteralNode> expr_;
  bool minus_;

  PatternLiteralNode(std::unique_ptr<ExprLiteralNode> &&expr, bool minus,
                     Position position);
  ~PatternLiteralNode();
  void accept(Visitor &visitor) override;
};