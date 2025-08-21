#pragma once
#include "ItemANode.hpp"

class TypeNode;
class ExprBlockNode;
class PatternNode;

struct ItemFnPara {
  std::unique_ptr<PatternNode> pattern;
  std::unique_ptr<TypeNode> type;
};

class ItemFnNode : public ItemNode {
public:
  std::string ID_;
  std::vector<ItemFnPara> parameters_;
  std::unique_ptr<TypeNode> return_type_;
  std::unique_ptr<ExprBlockNode> body_;
  bool const_flag;

  ItemFnNode(const std::string &ID, std::unique_ptr<TypeNode> &&return_type,
             std::vector<ItemFnPara> &&parameters,
             std::unique_ptr<ExprBlockNode> &&body,Position position);
  ~ItemFnNode();
  void accept(Visitor &visitor) override;
};