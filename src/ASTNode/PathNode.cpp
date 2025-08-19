#include "PathNode.hpp"
#include "Visitor.hpp"

PathNode::PathNode(std::vector<PathSegment> &&segments)
    : ASTNode(), segments_(std::move(segments)) {}
PathNode::~PathNode() = default;
void PathNode::accept(Visitor &visitor) { visitor.visit(this); }