#pragma once
#include "ExprANode.hpp"

class ExprTupleNode : public ExprBlockOutNode {
private:
  std::vector<std::unique_ptr<ExprNode>> elements_;

public:
  ExprTupleNode(std::vector<std::unique_ptr<ExprNode>> &&elements);
  ~ExprTupleNode();
  void accept(Visitor &visitor) override;
};