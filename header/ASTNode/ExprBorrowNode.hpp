#pragma once
#include "ExprANode.hpp"

class ExprBorrowNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> expr_;
  bool is_mutable_;

  ExprBorrowNode(std::unique_ptr<ExprNode> &&expr, bool is_mutable,
                 Position position);
  ~ExprBorrowNode() override;
  void accept(Visitor &visitor) override;
};