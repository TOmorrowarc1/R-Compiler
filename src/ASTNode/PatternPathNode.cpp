#include "PatternPathNode.hpp"
#include "PathNode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

PatternPathNode::PatternPathNode(std::unique_ptr<PathNode> &&path)
    : PatternNode(), path_(std::move(path)) {}
PatternPathNode::~PatternPathNode() = default;
void PatternPathNode::accept(Visitor &visitor) { visitor.visit(this); }