#pragma once
#include "TypeANode.hpp"

class ExprNode;

class TypeSliceNode : public TypeNode {
private:
  std::unique_ptr<TypeNode> type_;

public:
  TypeSliceNode(std::unique_ptr<TypeNode> &&type);
  ~TypeSliceNode();
  void accept(Visitor &visitor) override;
};