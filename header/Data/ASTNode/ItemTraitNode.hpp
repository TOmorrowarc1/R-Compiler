#pragma once
#include "ItemANode.hpp"

class ItemFnNode;
class ItemConstNode;
class TypeNode;

struct ItemAssociatedItem;

class ItemTraitNode : public ItemNode {
public:
  std::string trait_name_;
  std::vector<ItemAssociatedItem> items_;

  ItemTraitNode(const std::string &name,
                std::vector<ItemAssociatedItem> &&items, Position position);
  ~ItemTraitNode();
  void accept(Visitor &visitor) override;
};