#include "PatternWildNode.hpp"
#include "Visitor.hpp"

PatternWildNode::PatternWildNode() : PatternOneNode() {}
PatternWildNode::~PatternWildNode() = default;
void PatternWildNode::accept(Visitor &visitor) { visitor.visit(*this); }