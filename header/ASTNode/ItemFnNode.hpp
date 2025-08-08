#pragma once
#include "ItemANode.hpp"

class TypeNode;

struct ParameterNode {
  std::string name;
  std::unique_ptr<TypeNode> type;
};

class ItemFnNode : public ItemNode {
private:
  std::string ID_;
  std::unique_ptr<TypeNode> returnType_;
  std::vector<std::unique_ptr<ParameterNode>> parameters_;
public:
  ItemFnNode(const std::string &ID, std::unique_ptr<TypeNode> &&returnType,
             std::vector<std::unique_ptr<ParameterNode>> &&parameters);

  void accept(Visitor &visitor) override;
};