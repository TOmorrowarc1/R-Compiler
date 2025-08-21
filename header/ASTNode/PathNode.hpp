#pragma once
#include "ASTNode.hpp"

enum class PathSegmentType { IDENTIFER, SELF, SELF_TYPE };

struct PathSegment {
  PathSegmentType type;
  std::string name;
};

class PathNode : public ASTNode {
public:
  std::vector<PathSegment> segments_;

  PathNode(std::vector<PathSegment> &&segments, Position position);
  ~PathNode();
  void accept(Visitor &visitor) override;
};