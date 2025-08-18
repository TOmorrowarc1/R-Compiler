#pragma once
#include "PatternANode.hpp"

class PathNode;

class PatternPathNode : public PatternNode {
public:
  std::unique_ptr<PathNode> path_;

  PatternPathNode(std::unique_ptr<PathNode> &&path);
  ~PatternPathNode();
  void accept(Visitor &visitor) override;
};