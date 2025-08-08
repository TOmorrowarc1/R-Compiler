#pragma once
#include "TypeANode.hpp"

class TypePathNode : public TypeNode {
private:
  std::vector<std::string> path_;

public:
  TypePathNode(std::vector<std::string> path);
  ~TypePathNode() = default;
  void accept(Visitor &visitor) override;
};