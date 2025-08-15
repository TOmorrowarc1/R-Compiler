#include "PatternPathNode.hpp"
#include "PathANode.hpp"
#include "Visitor.hpp"

PatternPathNode::PatternPathNode(std::unique_ptr<PathNode> &&path)
    : PatternOneNode(), path_(std::move(path)) {}
PatternPathNode::~PatternPathNode() = default;
void PatternPathNode::accept(Visitor &visitor) { visitor.visit(*this); }