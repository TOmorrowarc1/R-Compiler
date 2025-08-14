#pragma once
#include "ExprANode.hpp"

class ExprFieldNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprNode> instance_;
  std::string ID_;

public:
  ExprFieldNode(std::unique_ptr<ExprNode> &&instance, const std::string &ID);
  ~ExprFieldNode() = default;
  void accept(Visitor &visitor) override;
};