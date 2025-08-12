#include "PatternPathNode.hpp"
#include "Visitor.hpp"

PatternPathNode::PatternPathNode(std::unique_ptr<PathNode> &&path)
    : PatternOneNode(), path_(std::move(path)) {}

void PatternPathNode::accept(Visitor &visitor) { visitor.visit(*this); }