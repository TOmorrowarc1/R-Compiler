#pragma once
#include "ExprANode.hpp"

class ExprPathNode;

struct ExprStructField {
  std::string ID;
  std::unique_ptr<ExprNode> expr;
};

class ExprStructNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprPathNode> path_;
  std::vector<ExprStructField> fields_;

  ExprStructNode(std::unique_ptr<ExprPathNode> &&path,
                 std::vector<ExprStructField> &&fields);
  ~ExprStructNode();
  void accept(Visitor &visitor) override;
};