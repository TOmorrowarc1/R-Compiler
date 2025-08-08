#include "StmtLetNode.hpp"
#include "Visitor.hpp"
StmtLetNode::StmtLetNode(std::string identifier, std::unique_ptr<TypeNode> type,
                         std::unique_ptr<ExprNode> init_value)
    : StmtNode(), identifier_(std::move(identifier)), type_(std::move(type)),
      init_value_(std::move(init_value)) {}
      
void StmtLetNode::accept(Visitor &visitor) { visitor.visit(*this); }