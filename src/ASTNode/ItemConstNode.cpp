#include "ItemConstNode.hpp"
#include "Visitor.hpp"

ItemConstNode::ItemConstNode(const std::string &ID,
                             std::unique_ptr<TypeNode> &&type,
                             std::unique_ptr<ExprNode> &&value)
    : ItemNode(), ID_(ID), type_(std::move(type)), value_(std::move(value)) {}

void ItemConstNode::accept(Visitor &visitor) { visitor.visit(*this); }