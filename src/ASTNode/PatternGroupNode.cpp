#include "PatternGroupNode.hpp"
#include "Visitor.hpp"

PatternGroupNode::PatternGroupNode(std::unique_ptr<PatternNode> &&expr)
    : PatternOneNode(), expr_(std::move(expr)) {}

void PatternGroupNode::accept(Visitor &visitor) { visitor.visit(*this); }