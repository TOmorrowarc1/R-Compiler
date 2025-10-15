#include "TypeArrayNode.hpp"
#include "ExprANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

TypeArrayNode::TypeArrayNode(std::unique_ptr<TypeNode> &&type,
                             std::unique_ptr<ExprNode> &&length,
                             Position position)
    : TypeNode(position), type_(std::move(type)), length_(std::move(length)) {}
TypeArrayNode::~TypeArrayNode() = default;
void TypeArrayNode::accept(Visitor &visitor) { visitor.visit(this); }