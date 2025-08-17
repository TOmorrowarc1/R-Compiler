#include "PatternStructNode.hpp"
#include "Visitor.hpp"

PatternStructNode::PatternStructNode(std::vector<PatternStructField> &&pattern)
    : PatternNode(), patterns_(std::move(pattern)) {}
PatternStructNode::~PatternStructNode() = default;
void PatternStructNode::accept(Visitor &visitor) { visitor.visit(*this); }