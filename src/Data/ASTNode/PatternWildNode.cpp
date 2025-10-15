#include "PatternWildNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

PatternWildNode::PatternWildNode(Position position) : PatternNode(position) {}
PatternWildNode::~PatternWildNode() = default;
void PatternWildNode::accept(Visitor &visitor) { visitor.visit(this); }