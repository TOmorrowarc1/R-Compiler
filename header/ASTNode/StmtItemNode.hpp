#pragma once
#include "StmtANode.hpp"

class ItemNode;
class StmtItemNode : public StmtNode {
public:
  std::unique_ptr<ItemNode> item_;

  StmtItemNode(std::unique_ptr<ItemNode> item);
  ~StmtItemNode();
  void accept(Visitor &visitor) override;
};