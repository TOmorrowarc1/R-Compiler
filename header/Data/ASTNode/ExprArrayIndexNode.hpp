#pragma once
#include "ExprANode.hpp"

class ExprArrayIndexNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> array_;
  std::unique_ptr<ExprNode> index_;

  ExprArrayIndexNode(std::unique_ptr<ExprNode> &&array,
                     std::unique_ptr<ExprNode> &&index, Position position);
  ~ExprArrayIndexNode();
  void accept(Visitor &visitor) override;
};