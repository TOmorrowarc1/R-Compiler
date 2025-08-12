#include "TypePathNode.hpp"
#include "Visitor.hpp"

TypePathNode::TypePathNode(std::unique_ptr<PathNode> &&path)
    : TypeNode(), path_(std::move(path)) {}

void TypePathNode::accept(Visitor &visitor) { visitor.visit(*this); }