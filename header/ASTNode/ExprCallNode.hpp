#pragma once
#include "ExprANode.hpp"

class ExprCallNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprNode> caller_;
  std::vector<std::unique_ptr<ExprNode>> arguments_;
public:
  ExprCallNode(std::unique_ptr<ExprNode> &&caller,
               std::vector<std::unique_ptr<ExprNode>> &&arguments);
  ~ExprCallNode() = default;
  void accept(Visitor &visitor) override;
};