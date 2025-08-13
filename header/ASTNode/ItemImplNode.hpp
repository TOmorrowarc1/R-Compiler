#pragma once
#include "ItemANode.hpp"

class ItemFunctionNode;
class ItemConstNode;
class TypeNode;

struct ItemAssociatedItem {
  std::unique_ptr<ItemFunctionNode> function;
  std::unique_ptr<ItemConstNode> constant;
};

class ItemImplNode : public ItemNode {
private:
  std::unique_ptr<TypeNode> type_;
  std::vector<ItemAssociatedItem> items_;

public:
  ItemImplNode(std::unique_ptr<TypeNode> &&type,
               std::vector<ItemAssociatedItem> &&items);
  ~ItemImplNode() = default;
  void accept(Visitor &visitor) override;
};