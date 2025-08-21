#pragma once
#include "ExprANode.hpp"

class PathNode;
class ExprPathNode : public ExprNode {
public:
  std::unique_ptr<PathNode> path_;

  ExprPathNode(std::unique_ptr<PathNode> &&path, Position position);
  ~ExprPathNode();
  void accept(Visitor &visitor) override;
};