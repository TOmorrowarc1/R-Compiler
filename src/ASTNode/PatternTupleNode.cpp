#include "PatternTupleNode.hpp"
#include "Visitor.hpp"

PatternTupleNode::PatternTupleNode(
    std::vector<std::unique_ptr<PatternNode>> &&pattern)
    : PatternNode(), patterns_(std::move(pattern)) {}
PatternTupleNode::~PatternTupleNode() = default;
void PatternTupleNode::accept(Visitor &visitor) { visitor.visit(*this); }