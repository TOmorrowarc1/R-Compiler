#include "PathQualifiedNode.hpp"
#include "Visitor.hpp"

PathQualifiedNode::PathQualifiedNode(std::vector<PathSegment> &&segments,
                                     std::unique_ptr<PathSimpleNode> &&parent,
                                     std::unique_ptr<TypeNode> &&type)
    : PathNode(), segments_(std::move(segments)), parent_(std::move(parent)),
      type_(std::move(type)) {}

void PathQualifiedNode::accept(Visitor &visitor) { visitor.visit(*this); }