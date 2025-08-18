#pragma once
#include "ItemANode.hpp"

class TypeNode;

struct ItemStructField {
  std::string identifier;
  std::unique_ptr<TypeNode> type;
};

class ItemStructNode : public ItemNode {
public:
  std::string ID_;
  std::vector<ItemStructField> fields_;

  ItemStructNode(const std::string &ID, std::vector<ItemStructField> &&fields);
  ~ItemStructNode();
  void accept(Visitor &visitor) override;
};