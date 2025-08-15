#include "ItemEnumNode.hpp"
#include "ExprANode.hpp"
#include "TypeANode.hpp"
#include "Visitor.hpp"

ItemEnumNode::ItemEnumNode(const std::string &ID,
                           std::vector<ItemEnumVariant> &&variants)
    : ItemNode(), ID_(ID), variants_(std::move(variants)) {}
ItemEnumNode::~ItemEnumNode() = default;
void ItemEnumNode::accept(Visitor &visitor) { visitor.visit(*this); }