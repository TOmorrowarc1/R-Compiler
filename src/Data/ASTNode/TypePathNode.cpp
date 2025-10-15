#include "TypePathNode.hpp"
#include "PathNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

TypePathNode::TypePathNode(std::unique_ptr<PathNode> &&path, Position position)
    : TypeNode(position), path_(std::move(path)) {}
TypePathNode::~TypePathNode() = default;
void TypePathNode::accept(Visitor &visitor) { visitor.visit(this); }