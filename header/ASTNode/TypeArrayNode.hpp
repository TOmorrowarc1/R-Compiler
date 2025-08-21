#pragma once
#include "TypeANode.hpp"

class ExprNode;

class TypeArrayNode : public TypeNode {
public:
  std::unique_ptr<TypeNode> type_;
  std::unique_ptr<ExprNode> length_;

  TypeArrayNode(std::unique_ptr<TypeNode> &&type,
                std::unique_ptr<ExprNode> &&length, Position position);
  ~TypeArrayNode();
  void accept(Visitor &visitor) override;
};