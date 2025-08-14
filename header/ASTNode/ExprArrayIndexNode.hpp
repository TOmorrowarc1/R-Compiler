#pragma once
#include "ExprANode.hpp"

class ExprArrayIndexNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprNode> array_;
  std::unique_ptr<ExprNode> index_;

public:
  ExprArrayIndexNode(std::unique_ptr<ExprNode> &&array,
                     std::unique_ptr<ExprNode> &&index);
  ~ExprArrayIndexNode() = default;
  void accept(Visitor &visitor) override;
};