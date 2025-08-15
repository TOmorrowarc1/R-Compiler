#pragma once
#include "PathANode.hpp"

class PathSimpleNode : public PathNode {
private:
  std::vector<PathSegment> segments_;

public:
  PathSimpleNode(std::vector<PathSegment> &&segments);
  ~PathSimpleNode();
  void accept(Visitor &visitor) override;
};