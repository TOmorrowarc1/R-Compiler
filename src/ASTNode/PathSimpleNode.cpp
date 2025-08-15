#include "PathSimpleNode.hpp"
#include "Visitor.hpp"

PathSimpleNode::PathSimpleNode(std::vector<PathSegment> &&segments)
    : PathNode(), segments_(std::move(segments)) {}
PathSimpleNode::~PathSimpleNode() = default;
void PathSimpleNode::accept(Visitor &visitor) { visitor.visit(*this); }