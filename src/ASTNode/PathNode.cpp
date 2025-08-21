#include "PathNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

PathNode::PathNode(std::vector<PathSegment> &&segments, Position position)
    : ASTNode(position), segments_(std::move(segments)) {}
PathNode::~PathNode() = default;
void PathNode::accept(Visitor &visitor) { visitor.visit(this); }