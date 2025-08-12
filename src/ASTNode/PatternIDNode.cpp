#include "PatternIDNode.hpp"
#include "Visitor.hpp"

PatternIDNode::PatternIDNode(const std::string &identifier)
    : PatternOneNode(), identifier_(identifier) {}

void PatternIDNode::accept(Visitor &visitor) { visitor.visit(*this); }