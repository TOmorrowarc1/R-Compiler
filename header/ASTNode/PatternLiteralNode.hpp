#pragma once
#include "PatternANode.hpp"

class ExprLiteralNode;

class PatternLiteralNode : public PatternOneNode {
private:
  std::unique_ptr<ExprLiteralNode> expr_;
  bool minus_;

public:
  PatternLiteralNode(std::unique_ptr<ExprLiteralNode> &&expr, bool minus);
  ~PatternLiteralNode() = default;
  void accept(Visitor &visitor) override;
};