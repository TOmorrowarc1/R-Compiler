#include "TypePathNode.hpp"
#include "PathNode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

TypePathNode::TypePathNode(std::unique_ptr<PathNode> &&path)
    : TypeNode(), path_(std::move(path)) {}
TypePathNode::~TypePathNode() = default;
void TypePathNode::accept(Visitor &visitor) { visitor.visit(this); }