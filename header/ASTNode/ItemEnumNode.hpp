#pragma once
#include "ItemStructNode.hpp"

class ItemEnumNode : public ItemNode {
private:
  std::string ID_;
  std::vector<std::string> variants_;

public:
  ItemEnumNode(const std::string &ID, std::vector<std::string> &&variants);
  ~ItemEnumNode();
  void accept(Visitor &visitor) override;
};
