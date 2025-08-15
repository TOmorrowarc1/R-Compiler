#pragma once
#include "ItemANode.hpp"

class TypeNode;
class ExprNode;

class ItemConstNode : public ItemNode {
private:
  std::string ID_;
  std::unique_ptr<TypeNode> type_;
  std::unique_ptr<ExprNode> value_;

public:
  ItemConstNode(const std::string &ID, std::unique_ptr<TypeNode> &&type,
                std::unique_ptr<ExprNode> &&value);
  ~ItemConstNode();
  void accept(Visitor &visitor) override;
};