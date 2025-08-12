#pragma once
#include "ASTNode.hpp"

enum class PathSegmentType { IDENTIFER, SELF, SELF_TYPE, SUPER, CRATE };

struct PathSegment {
  PathSegmentType type;
  std::string name;
};

class PathNode : public ASTNode {
private:
public:
  PathNode() : ASTNode() {}
  virtual ~PathNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};