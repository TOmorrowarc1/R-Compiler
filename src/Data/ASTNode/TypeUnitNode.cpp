#include "TypeUnitNode.hpp"
#include "Visitor.hpp"

TypeUnitNode::TypeUnitNode(Position position) : TypeNode(position) {}
TypeUnitNode::~TypeUnitNode() = default;
void TypeUnitNode::accept(Visitor &visitor) { visitor.visit(this); }