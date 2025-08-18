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
  std::unique_ptr<TypeNode> returnType_;
  std::unique_ptr<ExprBlockNode> body_;
  bool const_flag;

  ItemFnNode(const std::string &ID, std::unique_ptr<TypeNode> &&returnType,
             std::vector<ItemFnPara> &&parameters,
             std::unique_ptr<ExprBlockNode> &&body);
  ~ItemFnNode();
  void accept(Visitor &visitor) override;
};