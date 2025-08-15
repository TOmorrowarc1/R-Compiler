#include "TypeNeverNode.hpp"
#include "Visitor.hpp"

TypeNeverNode::TypeNeverNode() : TypeNode() {}
TypeNeverNode::~TypeNeverNode() = default;
void TypeNeverNode::accept(Visitor &visitor) { visitor.visit(*this); }