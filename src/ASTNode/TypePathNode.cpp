#include"TypePathNode.hpp"
#include "Visitor.hpp"

TypePathNode::TypePathNode(std::vector<std::string> path)
    : TypeNode(), path_(std::move(path)) {}

void TypePathNode::accept(Visitor &visitor) { visitor.visit(*this); }