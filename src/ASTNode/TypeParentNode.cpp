#include "TypeParentNode.hpp"
#include "Visitor.hpp"

TypeParentNode::TypeParentNode(std::unique_ptr<TypeNode> parentType)
    : TypeNode(), type_(std::move(parentType)) {}
TypeParentNode::~TypeParentNode() = default;
void TypeParentNode::accept(Visitor &visitor) { visitor.visit(*this); }