#pragma once
#include "ASTNode.hpp"

enum class PathSegmentType { IDENTIFER, SELF, SELF_TYPE};

struct PathSegment {
  PathSegmentType type;
  std::string name;
};

class PathNode : public ASTNode {
private:
  std::vector<PathSegment> segments_;

public:
  PathNode(std::vector<PathSegment> &&segments);
  ~PathNode();
  void accept(Visitor &visitor) override;
};