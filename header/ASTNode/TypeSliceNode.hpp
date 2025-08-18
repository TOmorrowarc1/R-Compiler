#pragma once
#include "TypeANode.hpp"

class ExprNode;

class TypeSliceNode : public TypeNode {
public:
  std::unique_ptr<TypeNode> type_;

  TypeSliceNode(std::unique_ptr<TypeNode> &&type);
  ~TypeSliceNode();
  void accept(Visitor &visitor) override;
};