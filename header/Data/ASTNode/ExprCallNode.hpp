#pragma once
#include "ExprANode.hpp"

class ExprCallNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> caller_;
  std::vector<std::unique_ptr<ExprNode>> arguments_;

  ExprCallNode(std::unique_ptr<ExprNode> &&caller,
               std::vector<std::unique_ptr<ExprNode>> &&arguments,
               Position position);
  ~ExprCallNode();
  void accept(Visitor &visitor) override;
};