#include "ItemTraitNode.hpp"
#include "ItemConstNode.hpp"
#include "ItemFnNode.hpp"
#include "ItemImplNode.hpp"
#include "Visitor.hpp"

ItemTraitNode::ItemTraitNode(const std::string &name,
                             std::vector<ItemAssociatedItem> &&items,
                             Position position)
    : ItemNode(position), trait_name_(name), items_(std::move(items)) {}
ItemTraitNode::~ItemTraitNode() = default;
void ItemTraitNode::accept(Visitor &visitor) { visitor.visit(this); }