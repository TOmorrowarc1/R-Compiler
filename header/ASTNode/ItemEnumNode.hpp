#pragma once
#include "ItemStructNode.hpp"

class ItemEnumNode : public ItemNode {
public:
  std::string ID_;
  std::vector<std::string> variants_;

  ItemEnumNode(const std::string &ID, std::vector<std::string> &&variants);
  ~ItemEnumNode();
  void accept(Visitor &visitor) override;
};
