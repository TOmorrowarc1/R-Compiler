#pragma once
#include "TypeANode.hpp"

class PathNode;

class TypePathNode : public TypeNode {
private:
  std::unique_ptr<PathNode> path_;

public:
  TypePathNode(std::unique_ptr<PathNode> &&path);
  ~TypePathNode();
  void accept(Visitor &visitor) override;
};