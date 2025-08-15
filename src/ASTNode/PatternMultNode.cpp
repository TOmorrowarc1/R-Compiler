#include "PatternANode.hpp"
#include "Visitor.hpp"

PatternMultNode::PatternMultNode(
    std::vector<std::unique_ptr<PatternOneNode>> &&patterns)
    : PatternNode(), patterns_(std::move(patterns)) {}
PatternMultNode::~PatternMultNode() = default;
void PatternMultNode::accept(Visitor &visitor) { visitor.visit(*this); }