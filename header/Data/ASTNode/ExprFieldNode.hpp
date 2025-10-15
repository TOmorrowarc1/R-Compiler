#pragma once
#include "ExprANode.hpp"

class ExprFieldNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> instance_;
  std::string ID_;

  ExprFieldNode(std::unique_ptr<ExprNode> &&instance, const std::string &ID,
                Position position);
  ~ExprFieldNode();
  void accept(Visitor &visitor) override;
};