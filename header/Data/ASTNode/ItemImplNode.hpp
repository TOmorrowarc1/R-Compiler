#pragma once
#include "ItemANode.hpp"

class ItemFnNode;
class ItemConstNode;
class TypeNode;

struct ItemAssociatedItem {
  std::unique_ptr<ItemFnNode> function;
  std::unique_ptr<ItemConstNode> constant;
};

class ItemImplNode : public ItemNode {
public:
  std::unique_ptr<TypeNode> type_;
  std::vector<ItemAssociatedItem> items_;
  std::string trait_name_;

  ItemImplNode(std::unique_ptr<TypeNode> &&type,
               std::vector<ItemAssociatedItem> &&items, const std::string &name,
               Position position);
  ~ItemImplNode();
  void accept(Visitor &visitor) override;
};