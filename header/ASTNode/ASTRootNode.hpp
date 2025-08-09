#pragma once
#include "ASTNode.hpp"

class ItemNode;

class ASTRootNode : public ASTNode {
private:
  std::vector<std::unique_ptr<ItemNode>> items_;

public:
  ASTRootNode(std::vector<std::unique_ptr<ItemNode>> &&items);
  ~ASTRootNode() = default;
  void accept(Visitor &visitor) override;
};