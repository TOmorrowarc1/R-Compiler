#include "ItemEnumNode.hpp"
#include "Visitor.hpp"

ItemEnumNode::ItemEnumNode(const std::string &ID,
                           std::vector<ItemEnumVariant> &&variants)
    : ItemNode(), ID_(ID), variants_(std::move(variants)) {}

void ItemEnumNode::accept(Visitor &visitor) { visitor.visit(*this); }