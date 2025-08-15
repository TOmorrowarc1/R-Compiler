#pragma once
#include "ExprANode.hpp"

class PathNode;
class ExprPathNode : public ExprNode {
private:
  std::unique_ptr<PathNode> path_;

public:
  ExprPathNode(std::unique_ptr<PathNode>&& path);
  ~ExprPathNode();
  void accept(Visitor &visitor) override;
};