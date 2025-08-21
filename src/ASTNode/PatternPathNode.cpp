#include "PatternPathNode.hpp"
#include "PathNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

PatternPathNode::PatternPathNode(std::unique_ptr<PathNode> &&path,
                                 Position position)
    : PatternNode(position), path_(std::move(path)) {}
PatternPathNode::~PatternPathNode() = default;
void PatternPathNode::accept(Visitor &visitor) { visitor.visit(this); }