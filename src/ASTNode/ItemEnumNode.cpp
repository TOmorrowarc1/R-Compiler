#include "ItemEnumNode.hpp"
#include "Visitor.hpp"

ItemEnumNode::ItemEnumNode(const std::string &ID,
                           std::vector<std::string> &&variants)
    : ItemNode(), ID_(ID), variants_(std::move(variants)) {}
ItemEnumNode::~ItemEnumNode() = default;
void ItemEnumNode::accept(Visitor &visitor) { visitor.visit(this); }