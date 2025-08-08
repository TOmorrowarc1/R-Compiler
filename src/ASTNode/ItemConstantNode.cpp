#include "ItemConstantNode.hpp"
#include "Visitor.hpp"

ItemConstantNode::ItemConstantNode(const std::string &ID,
                                   std::unique_ptr<TypeNode> &&type,
                                   std::unique_ptr<ExprNode> &&value)
    : ItemNode(), ID_(ID), type_(std::move(type)), value_(std::move(value)) {}

void ItemConstantNode::accept(Visitor &visitor) { visitor.visit(*this); }