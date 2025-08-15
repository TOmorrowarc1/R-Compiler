#pragma once
#include "ExprANode.hpp"

class ExprPathNode;

struct ExprStructField {
  std::string ID;
  std::unique_ptr<ExprNode> expr;
};

class ExprStructNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprPathNode> path_;
  std::vector<ExprStructField> fields_;

public:
  ExprStructNode(std::unique_ptr<ExprPathNode> &&path,
                 std::vector<ExprStructField> &&fields);
  ~ExprStructNode();
  void accept(Visitor &visitor) override;
};