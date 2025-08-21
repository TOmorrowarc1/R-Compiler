#include "ExprANode.hpp"

class ExprDereferNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> expr_;
  
  ExprDereferNode(std::unique_ptr<ExprNode> &&expr, Position position);
  ~ExprDereferNode() override;
  void accept(Visitor &visitor) override;
};