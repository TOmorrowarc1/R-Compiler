#pragma once
#include "ExprANode.hpp"

class ExprTupleNode : public ExprBlockOutNode {
public:
  std::vector<std::unique_ptr<ExprNode>> elements_;

  ExprTupleNode(std::vector<std::unique_ptr<ExprNode>> &&elements);
  ~ExprTupleNode();
  void accept(Visitor &visitor) override;
};