#pragma once
#include "ItemANode.hpp"

class TypeNode;
class ExprNode;

class ItemConstNode : public ItemNode {
public:
  std::string ID_;
  std::unique_ptr<TypeNode> type_;
  std::unique_ptr<ExprNode> value_;

  ItemConstNode(const std::string &ID, std::unique_ptr<TypeNode> &&type,
                std::unique_ptr<ExprNode> &&value, Position position);
  ~ItemConstNode();
  void accept(Visitor &visitor) override;
};