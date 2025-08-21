#include "ExprPathNode.hpp"
#include "PathNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprPathNode::ExprPathNode(std::unique_ptr<PathNode> &&path, Position position)
    : ExprNode(position), path_(std::move(path)) {}
ExprPathNode::~ExprPathNode() = default;
void ExprPathNode::accept(Visitor &visitor) { visitor.visit(this); }