#pragma once
#include "ItemANode.hpp"

class TypeNode;
class ExprNode;

class ItemConstantNode : public ItemNode {
private:
  std::string ID_;
  std::unique_ptr<TypeNode> type_;
  std::unique_ptr<ExprNode> value_;

public:
  ItemConstantNode(const std::string &ID, std::unique_ptr<TypeNode> &&type,
                   std::unique_ptr<ExprNode> &&value);

  void accept(Visitor &visitor) override;
};