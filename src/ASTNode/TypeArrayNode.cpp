#include "TypeArrayNode.hpp"
#include "ExprANode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

TypeArrayNode::TypeArrayNode(std::unique_ptr<TypeNode> &&type,
                             std::unique_ptr<ExprNode> &&length)
    : TypeNode(), type_(std::move(type)), length_(std::move(length)) {}
TypeArrayNode::~TypeArrayNode() = default;
void TypeArrayNode::accept(Visitor &visitor) { visitor.visit(this); }