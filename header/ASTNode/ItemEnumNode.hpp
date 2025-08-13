#pragma once
#include "ItemStructNode.hpp"

class TypeNode;
class ExprNode;

struct ItemTupleField {
  std::unique_ptr<TypeNode> type;
};

struct ItemEnumVariant {
  std::string ID;
  std::vector<ItemTupleField> tuple_fields;
  std::vector<ItemStructField> struct_fields;
  std::unique_ptr<ExprNode> discriminant;
};

class ItemEnumNode : public ItemNode {
private:
  std::string ID_;
  std::vector<ItemEnumVariant> variants_;

public:
  ItemEnumNode(const std::string &ID, std::vector<ItemEnumVariant> &&variants);
  ~ItemEnumNode() = default;
  void accept(Visitor &visitor) override;
};
