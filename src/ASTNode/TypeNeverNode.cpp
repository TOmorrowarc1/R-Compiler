#include "TypeNeverNode.hpp"
#include "Visitor.hpp"

TypeNeverNode::TypeNeverNode() : TypeNode() {}
void TypeNeverNode::accept(Visitor &visitor) { visitor.visit(*this); }