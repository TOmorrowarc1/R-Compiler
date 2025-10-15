#pragma once
#include "StmtANode.hpp"

class ItemNode;
class StmtItemNode : public StmtNode {
public:
  std::unique_ptr<ItemNode> item_;

  StmtItemNode(std::unique_ptr<ItemNode> &&item, Position position);
  ~StmtItemNode();
  void accept(Visitor &visitor) override;
};