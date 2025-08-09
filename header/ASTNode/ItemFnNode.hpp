#pragma once
#include "ItemANode.hpp"

class TypeNode;
class ExprBlockNode;

struct FnParameter {
  std::string name;
  std::unique_ptr<TypeNode> type;
};

class ItemFnNode : public ItemNode {
private:
  std::string ID_;
  std::unique_ptr<TypeNode> returnType_;
  std::vector<FnParameter> parameters_;
  std::unique_ptr<ExprBlockNode> body_;

public:
  ItemFnNode(const std::string &ID, std::unique_ptr<TypeNode> &&returnType,
             std::vector<FnParameter> &&parameters,
             std::unique_ptr<ExprBlockNode> &&body);
  ~ItemFnNode() = default;
  void accept(Visitor &visitor) override;
};