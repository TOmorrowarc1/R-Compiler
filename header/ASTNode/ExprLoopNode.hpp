#pragma once
#include "ExprANode.hpp"

class ExprLoopNode : public ExprBlockInNode {
private:
  std::unique_ptr<ExprBlockInNode> loop_body_;

public:
  ExprLoopNode(std::unique_ptr<ExprBlockInNode> &&loop_body);
  ~ExprLoopNode() = default;
  void accept(Visitor &visitor) override;
};

class ExprWhileNode : public ExprBlockInNode {
private:
  std::unique_ptr<ExprNode> condition_;
  std::unique_ptr<ExprBlockInNode> loop_body_;
public:
  ExprWhileNode(std::unique_ptr<ExprNode> &&condition,
                std::unique_ptr<ExprBlockInNode> &&loop_body);
  ~ExprWhileNode() = default;
  void accept(Visitor &visitor) override;
};