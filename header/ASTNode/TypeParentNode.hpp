#pragma once
#include"TypeANode.hpp"

class TypeParentNode : public TypeNode {
private:
  std::unique_ptr<TypeNode> type_;
public:
  TypeParentNode(std::unique_ptr<TypeNode> parentType);
  ~TypeParentNode() = default;
  void accept(Visitor &visitor) override;
};