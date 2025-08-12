#pragma once
#include "TypeANode.hpp"

class ExprNode;

class TypeTupleNode : public TypeNode {
private:
  std::vector<std::unique_ptr<TypeNode>> fields_;

public:
  TypeTupleNode(std::vector<std::unique_ptr<TypeNode>> &&fields);
  ~TypeTupleNode() = default;
  void accept(Visitor &visitor) override;
};