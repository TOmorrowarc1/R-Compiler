#include "ItemModuleNode.hpp"
#include "Visitor.hpp"

ItemModuleNode::ItemModuleNode(const std::string &ID,
                               std::vector<std::unique_ptr<ItemNode>> &&items)
    : ItemNode(), ID_(ID), items_(items) {}

void ItemModuleNode::accept(Visitor &visitor) { visitor.visit(*this); }