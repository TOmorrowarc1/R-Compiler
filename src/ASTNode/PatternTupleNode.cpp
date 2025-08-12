#include "PatternTupleNode.hpp"
#include "Visitor.hpp"

PatternTupleNode::PatternTupleNode(
    std::vector<std::unique_ptr<PatternNode>> &&pattern)
    : PatternOneNode(), patterns_(std::move(pattern)) {}

void PatternTupleNode::accept(Visitor &visitor) { visitor.visit(*this); }