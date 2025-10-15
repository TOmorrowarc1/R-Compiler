#include "ItemImplNode.hpp"
#include "ItemConstNode.hpp"
#include "ItemFnNode.hpp"
#include "TypeANode.hpp"
#include "Visitor.hpp"

ItemImplNode::ItemImplNode(std::unique_ptr<TypeNode> &&type,
                           std::vector<ItemAssociatedItem> &&items,
                           const std::string &name, Position position)
    : ItemNode(position), type_(std::move(type)), items_(std::move(items)),
      trait_name_(name) {}
ItemImplNode::~ItemImplNode() = default;
void ItemImplNode::accept(Visitor &visitor) { visitor.visit(this); }
