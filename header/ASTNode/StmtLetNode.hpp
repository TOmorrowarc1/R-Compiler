#pragma once
#include "StmtANode.hpp"

class TypeNode;
class ExprNode;

class StmtLetNode : public StmtNode {
private:
  std::string identifier_;
  std::unique_ptr<TypeNode> type_;
  std::unique_ptr<ExprNode> init_value_;
public:
  StmtLetNode(std::string identifier, std::unique_ptr<TypeNode> type,
              std::unique_ptr<ExprNode> init_value);
  ~StmtLetNode() = default;
  void accept(Visitor &visitor) override;
};