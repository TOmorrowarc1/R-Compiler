#pragma once
#include "StmtANode.hpp"

class ItemNode;
class StmtItemNode : public StmtNode {
private:
  std::unique_ptr<ItemNode> item_;
public:
  StmtItemNode(std::unique_ptr<ItemNode> item);
  ~StmtItemNode() = default;
  void accept(Visitor &visitor) override ;
};