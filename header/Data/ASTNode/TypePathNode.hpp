#pragma once
#include "TypeANode.hpp"

class PathNode;

class TypePathNode : public TypeNode {
public:
  std::unique_ptr<PathNode> path_;

  TypePathNode(std::unique_ptr<PathNode> &&path, Position position);
  ~TypePathNode();
  void accept(Visitor &visitor) override;
};