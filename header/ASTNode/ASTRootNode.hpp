#pragma once
#include "ASTNode.hpp"

class ItemNode;

class ASTRootNode : public ASTNode {
public:
  std::vector<std::unique_ptr<ItemNode>> items_;

  ASTRootNode(std::vector<std::unique_ptr<ItemNode>> &&items,
              Position position);
  ~ASTRootNode();
  void accept(Visitor &visitor) override;
};