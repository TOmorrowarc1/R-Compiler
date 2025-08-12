#include "PatternSliceNode.hpp"
#include "Visitor.hpp"

PatternSliceNode::PatternSliceNode(
    std::vector<std::unique_ptr<PatternNode>> &&pattern)
    : PatternOneNode(), patterns_(std::move(pattern)) {}

void PatternSliceNode::accept(Visitor &visitor) { visitor.visit(*this); }