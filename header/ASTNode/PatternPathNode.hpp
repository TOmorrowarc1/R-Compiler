#pragma once
#include "PatternANode.hpp"

class PathNode;

class PatternPathNode : public PatternOneNode {
private:
  std::unique_ptr<PathNode> path_;

public:
  PatternPathNode(std::unique_ptr<PathNode> &&path);
  ~PatternPathNode() = default;
  void accept(Visitor &visitor) override;
};