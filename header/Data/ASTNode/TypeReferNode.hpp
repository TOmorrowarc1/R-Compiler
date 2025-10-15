#pragma once
#include "TypeANode.hpp"

class TypeReferNode : public TypeNode {
public:
  std::unique_ptr<TypeNode> type_;
  bool is_mutable_;

  TypeReferNode(std::unique_ptr<TypeNode> &&type, bool is_mutable,
                Position position);
  ~TypeReferNode() override;
  void accept(Visitor &visitor) override;
};