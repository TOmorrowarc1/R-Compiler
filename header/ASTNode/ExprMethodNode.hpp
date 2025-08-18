#pragma once
#include "ExprANode.hpp"

class ExprMethodNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> instance_;
  std::string ID_;
  std::vector<std::unique_ptr<ExprNode>> parameters_;

  ExprMethodNode(std::unique_ptr<ExprNode> &&instance, const std::string &ID,
                 std::vector<std::unique_ptr<ExprNode>> &&parameters);
  ~ExprMethodNode();
  void accept(Visitor &visitor) override;
};