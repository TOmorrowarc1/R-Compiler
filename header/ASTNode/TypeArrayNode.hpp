#pragma once
#include "TypeANode.hpp"

class ExprNode;

class TypeArrayNode : public TypeNode {
private:
  std::unique_ptr<TypeNode> type_;
  std::unique_ptr<ExprNode> length_;

public:
  TypeArrayNode(std::unique_ptr<TypeNode> &&type,
                std::unique_ptr<ExprNode> &&length);
  ~TypeArrayNode() = default;
  void accept(Visitor &visitor) override;
};