#include "TypeReferNode.hpp"
#include "Visitor.hpp"

TypeReferNode::TypeReferNode(std::unique_ptr<TypeNode> &&type, bool is_mutable,
                             Position position)
    : TypeNode(position), is_mutable_(is_mutable), type_(std::move(type)) {}
TypeReferNode::~TypeReferNode() = default;
void TypeReferNode::accept(Visitor &visitor) { visitor.visit(this); }