#pragma once
#include "TypeANode.hpp"

class TypeInferNode : public TypeNode {
private:
public:
  TypeInferNode();
  ~TypeInferNode() = default;
  void accept(Visitor &visitor) override;
};