#pragma once
#include "ExprANode.hpp"

class ExprArrayNode : public ExprBlockOutNode {
public:
  std::vector<std::unique_ptr<ExprNode>> elements_;
  std::unique_ptr<ExprNode> length_;

  ExprArrayNode(std::vector<std::unique_ptr<ExprNode>> &&elements,
                std::unique_ptr<ExprNode> &&length);
  ~ExprArrayNode();
  void accept(Visitor &visitor) override;
};