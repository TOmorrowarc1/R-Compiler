#pragma once
#include "ExprANode.hpp"

class ExprLiteralIntNode;

class ExprTupleIndexNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> tuple_;
  std::unique_ptr<ExprLiteralIntNode> index_;

  ExprTupleIndexNode(std::unique_ptr<ExprNode> &&tuple,
                     std::unique_ptr<ExprLiteralIntNode> &&index);
  ~ExprTupleIndexNode();
  void accept(Visitor &visitor) override;
};