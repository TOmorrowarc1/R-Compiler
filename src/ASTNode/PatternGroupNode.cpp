#include "PatternGroupNode.hpp"
#include "Visitor.hpp"

PatternGroupNode::PatternGroupNode(std::unique_ptr<PatternNode> &&pattern)
    : PatternOneNode(), pattern_(std::move(pattern)) {}
PatternGroupNode::~PatternGroupNode() = default;
void PatternGroupNode::accept(Visitor &visitor) { visitor.visit(*this); }