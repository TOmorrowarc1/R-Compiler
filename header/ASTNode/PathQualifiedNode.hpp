#pragma once
#include "PathANode.hpp"

class PathSimpleNode;
class TypeNode;

class PathQualifiedNode : public PathNode {
private:
  std::vector<PathSegment> segments_;
  std::unique_ptr<PathSimpleNode> parent_;
  std::unique_ptr<TypeNode> type_;

public:
  PathQualifiedNode(std::vector<PathSegment> &&segments,
                    std::unique_ptr<PathSimpleNode> &&parent,
                    std::unique_ptr<TypeNode> &&type);
  ~PathQualifiedNode() override = default;
  void accept(Visitor &visitor) override;
};