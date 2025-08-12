#include "PatternStructNode.hpp"
#include "Visitor.hpp"

PatternStructNode::PatternStructNode(std::vector<PatternStructField> &&pattern)
    : PatternOneNode(), patterns_(std::move(pattern)) {}

void PatternStructNode::accept(Visitor &visitor) { visitor.visit(*this); }