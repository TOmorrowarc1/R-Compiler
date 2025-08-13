#pragma once
#include "ItemANode.hpp"

class TypeNode;
class ExprBlockNode;
class PatternOneNode;

struct FnParameter {
  std::unique_ptr<PatternOneNode> pattern;
  std::unique_ptr<TypeNode> type;
};

class ItemFnNode : public ItemNode {
private:
  std::string ID_;
  std::vector<FnParameter> parameters_;
  std::unique_ptr<TypeNode> returnType_;
  std::unique_ptr<ExprBlockNode> body_;
  bool const_flag;

public:
  ItemFnNode(const std::string &ID, std::unique_ptr<TypeNode> &&returnType,
             std::vector<FnParameter> &&parameters,
             std::unique_ptr<ExprBlockNode> &&body);
  ~ItemFnNode() = default;
  void accept(Visitor &visitor) override;
};