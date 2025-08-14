#pragma once
#include "ExprANode.hpp"

class ExprMethodNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprNode> instance_;
  std::string ID_;
  std::vector<std::unique_ptr<ExprNode>> parameters_;

public:
  ExprMethodNode(std::unique_ptr<ExprNode> &&instance, const std::string &ID,
                 std::vector<std::unique_ptr<ExprNode>> &&parameters);
  ~ExprMethodNode() = default;
  void accept(Visitor &visitor) override;
};