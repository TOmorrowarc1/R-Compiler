#include "PatternANode.hpp"
#include "Visitor.hpp"

PatternMultNode::PatternMultNode(
    std::vector<std::unique_ptr<PatternOneNode>> &&patterns)
    : PatternNode(), patterns_(patterns){};

void PatternMultNode::accept(Visitor &visitor) { visitor.visit(*this); }