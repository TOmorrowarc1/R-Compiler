#include "PatternWildNode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

PatternWildNode::PatternWildNode() : PatternNode() {}
PatternWildNode::~PatternWildNode() = default;
void PatternWildNode::accept(Visitor &visitor) { visitor.visit(this); }