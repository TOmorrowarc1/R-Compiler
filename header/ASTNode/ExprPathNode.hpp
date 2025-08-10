#pragma once
#include "ExprANode.hpp"

class ExprPathNode : public ExprNode {
private:
  std::string path_;

public:
  ExprPathNode(const std::string &path);
  ~ExprPathNode() override = default;
  void accept(Visitor &visitor) override;
};