#include "PatternWildNode.hpp"
#include "Visitor.hpp"

PatternWildNode::PatternWildNode() : PatternOneNode() {}

void PatternWildNode::accept(Visitor &visitor) { visitor.visit(*this); }