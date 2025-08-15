#include "ItemImplNode.hpp"
#include "ItemConstNode.hpp"
#include "ItemFnNode.hpp"
#include "TypeANode.hpp"
#include "Visitor.hpp"

ItemImplNode::ItemImplNode(std::unique_ptr<TypeNode> &&type,
                           std::vector<ItemAssociatedItem> &&items)
    : type_(std::move(type)), items_(std::move(items)) {}
ItemImplNode::~ItemImplNode() = default;
void ItemImplNode::accept(Visitor &visitor) { visitor.visit(*this); }
