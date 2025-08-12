#include "TypeArrayNode.hpp"
#include "Visitor.hpp"

TypeArrayNode::TypeArrayNode(std::unique_ptr<TypeNode> &&type,
                             std::unique_ptr<ExprNode> &&length)
    : TypeNode(), type_(std::move(type)), length_(std::move(length)) {}

void TypeArrayNode::accept(Visitor &visitor) { visitor.visit(*this); }