#include "ItemStructNode.hpp"
#include "Visitor.hpp"

ItemStructNode::ItemStructNode(const std::string &ID,
                               std::vector<ItemStructField> &&fields)
    : ItemNode(), ID_(ID), fields_(std::move(fields)) {}

void ItemStructNode::accept(Visitor &visitor) { visitor.visit(*this); }