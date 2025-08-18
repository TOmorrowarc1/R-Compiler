#pragma once
#include "PatternANode.hpp"

class PathNode;

class PatternTupleNode : public PatternNode {
public:
  std::unique_ptr<PathNode> path_;
  std::vector<std::unique_ptr<PatternNode>> patterns_;

  PatternTupleNode(std::unique_ptr<PathNode> path,
                   std::vector<std::unique_ptr<PatternNode>> &&pattern);
  ~PatternTupleNode();
  void accept(Visitor &visitor) override;
};