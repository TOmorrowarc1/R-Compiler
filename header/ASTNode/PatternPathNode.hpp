#pragma once
#include "PatternANode.hpp"

class PathNode;

class PatternPathNode : public PatternNode {
private:
  std::unique_ptr<PathNode> path_;

public:
  PatternPathNode(std::unique_ptr<PathNode> &&path);
  ~PatternPathNode();
  void accept(Visitor &visitor) override;
};