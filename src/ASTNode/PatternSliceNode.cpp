#include "PatternSliceNode.hpp"
#include "Visitor.hpp"

PatternSliceNode::PatternSliceNode(
    std::vector<std::unique_ptr<PatternNode>> &&pattern)
    : PatternOneNode(), patterns_(std::move(pattern)) {}
PatternSliceNode::~PatternSliceNode() = default;
void PatternSliceNode::accept(Visitor &visitor) { visitor.visit(*this); }